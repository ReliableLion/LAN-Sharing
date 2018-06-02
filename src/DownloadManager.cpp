#include "stdafx.h"

#include "DownloadManager.hpp"

DownloadManager::DownloadManager() {
    is_terminated_.store(false);

    for (int i = 0; i < max_thread_b_; i++) {                                                   // threads declarations for the big file download
        thread_pool_b_.push_back(std::thread(&DownloadManager::process_big_file, this));        // pass to these threads the process big file methods
    }

    for (int i = 0; i < max_thread_s_; i++) {                                                    // threads declarations for the small file download
        thread_pool_s_.push_back(std::thread(&DownloadManager::process_small_file, this));       // pass to these threads the process small file methods
    }
}

DownloadManager::~DownloadManager() {
    terminate_service();
}

void DownloadManager::terminate_service() {
    is_terminated_.store(true);                                // set is terminated true in order to stop the threds execution

    // join all the threads
    for (int i = 0; i < max_thread_s_; i++) {
        cv_s_.notify_all();                                    // "unlock" all the threads that waits on the condition variable
        thread_pool_s_[i].join();
    }

    // join all the threads
    for (int i = 0; i < max_thread_b_; i++) {
        cv_b_.notify_all();                                    // "unlock" all the threads that waits on the condition variable
        thread_pool_b_[i].join();
    }
}

bool DownloadManager::insert_big_file(request_struct request, connection::conn_ptr connection) {
    download_struct newRequest;
    bool queue_insertion_res;

    // fill the request struct with the date received by the client
    newRequest.req = request;
    newRequest.conn = connection;

    std::unique_lock<std::mutex> ul(mtx_b_);
    queue_insertion_res = big_file_q_.insertElement(newRequest);

    if (queue_insertion_res)                                        // if the connection is insert correctly into the queue notify it to the threads and then return true otherwise return false;
    {
        cv_b_.notify_all();
        return true;
    }

    return false;
}

bool DownloadManager::insert_small_file(request_struct request, connection::conn_ptr connection) {
    download_struct newRequest;
    bool queue_insertion_res;

    newRequest.req = request;
    newRequest.conn = connection;

    std::unique_lock<std::mutex> ul(mtx_s_);
    queue_insertion_res = small_file_q_.insertElement(newRequest);

    if (queue_insertion_res) {
        cv_s_.notify_all();
        return true;
    }

    return false;
}

void DownloadManager::process_small_file() {
    std::unique_lock<std::mutex> ul(mtx_s_, std::defer_lock);
    download_struct smallFileReq;
    bool exit = false;

    while (exit) {
        ul.lock();

        cv_s_.wait(ul, [this] {
            return (!small_file_q_.isEmpty() && !is_terminated_.load());
        });

        // get the request struct from the queue than release the lock
        if (is_terminated_.load()) exit = true;
        else small_file_q_.popElement(smallFileReq);

        // release the queue lock
        ul.unlock();
        /*
        try
        {
            if (!exit)
            {

                // create a temporary file for the download
                TemporaryFile temporary_file();
                if (download_file(smallFileReq, temporary_file()))
                {

                    FileHandler destination_file();
                    if (!copy_file(temporary_file, destination_file))
                    {
                        destination_file.remove();
                        std::cout << "impossible to copy the file to destination" << std::endl;
                    }
                }
                else
                {
                    std::cout << "impossible to complete the download of the file..." << std::endl;
                }

            }

        }
        catch (SocketException &se) {
            std::cout << "server error: " << se.what() << std::endl;
        }
        catch (FileWriteException &fwe) {
            std::cout << "impossible to write the data into the specified file" << std::endl;
        }
        catch (TimeoutException &te) {
            std::cout << "connection reached timeout, closing the connection" << std::endl;
        }

        smallFileReq.conn->close_connection(); */
    }
}

void DownloadManager::process_big_file() {
    std::unique_lock<std::mutex> ul(mtx_b_, std::defer_lock);
    download_struct bigFileReq;
    bool exit = false;

    while (exit) {
        ul.lock();

        cv_b_.wait(ul, [this] {
            return (!big_file_q_.isEmpty() && !is_terminated_.load());
        });

        if (is_terminated_.load()) exit = true;
        else small_file_q_.popElement(bigFileReq);

        ul.unlock();
        /*
        try
        {
            if (!exit)
            {
                // create a new temp file that
                TemporaryFile temporary_file(bigFileReq.req.file_name, temp_path_);

                if (download_file(bigFileReq, temporary_file))
                {
                    // create aa new file and copy the content the temporary one it into this
                    FileHandler destination_file();
                    if (!copy_file(temporary_file, destination_file))
                    {
                        destination_file.remove();
                        std::cout << "impossible to copy the file to the destination folder";
                    }

                }
                else
                    std::cout << "impossible to complete the download of the file..." << std::endl;

            }
        } catch (SocketException &se)
        {
            std::cout << "server error: " << se.what() << std::endl;
        } catch (TimeoutException &te)
        {
            std::cout << "connection reached timeout, closing the connection" << std::endl;
        }

        bigFileReq.conn->close_connection(); */
    }
}

bool DownloadManager::download_file(download_struct request, TemporaryFile &temporary_file) {
    int left_bytes = (int) request.req.file_size;
    int bytes_to_download = 0, downloaded_bytes = 0;
    bool connection_closed = false;

    std::shared_ptr<SocketBuffer> buffer;
    const int buffer_max_size = buffer->get_max_size();

    try {
        temporary_file.open_file(WRITE);                                // open the two files, if an exception is throw by the program then the file is closed by the destructor

        while (left_bytes != 0 && !connection_closed) {
            if (left_bytes >= buffer_max_size)                    // if the remaining data are greater than the max size of the buffer then the bytes to download are max buff lengh
                bytes_to_download = buffer_max_size;
            else
                bytes_to_download = left_bytes;                // if the remaining data are smaller than the max, set the remaining bytes value

            if (request.conn->read_data(buffer, bytes_to_download))            // check if the connection is
            {
                left_bytes -= buffer->get_size();
                temporary_file.write_data(buffer);
            } else connection_closed = true;

        }

        if (left_bytes != 0) return false;

        return true;

    } catch (FileOpenException &foe) {
        // TODO fare qualcosa
        return false;
    } catch (FileWriteException &fwe) {
        // TODO fare qualcosa
        return false;
    }
}

bool DownloadManager::copy_file(TemporaryFile &temporary_file, FileHandler &destination_file) {
    destination_file.open_file(WRITE);

    if (destination_file.copy_file(temporary_file)) {
        temporary_file.close_file();
        destination_file.close_file();
    } else
        return false;

    return true;
}
