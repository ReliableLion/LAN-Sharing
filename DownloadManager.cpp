#include <iostream>

#include "DownloadManager.hpp"
#include "Exceptions.hpp"
#include "PacketDispatcher.hpp"
#include "ConcurrentStreamPrint.hpp"

using namespace connection;

DownloadManager::DownloadManager() {
    is_terminated_.store(false);

#ifdef APP_DEBUG
	std::cout << "Download manager is running" << std::endl;
#endif 

    for (auto i = 0; i < MAX_THREAD_B_; i++) {														// threads declarations for the big file download
        thread_pool_b_.push_back(std::thread(&DownloadManager::process_big_file, this, i));			// pass to these threads the process big file methods
    }

    for (auto i = 0; i < MAX_THREAD_S_; i++) {														// threads declarations for the small file download
        thread_pool_s_.push_back(std::thread(&DownloadManager::process_small_file, this, MAX_THREAD_B_ + i));			// pass to these threads the process small file methods
    }
}

DownloadManager::~DownloadManager() {
    terminate_service();
}

void DownloadManager::terminate_service() {
    is_terminated_.store(true);                                // set is terminated true in order to stop the threds execution

    // join all threads
    for (auto i = 0; i < MAX_THREAD_S_; i++) {
        cv_s_.notify_all();                                    // "unlock" all the threads that waits on the condition variable
        thread_pool_s_[i].join();
    }

    for (auto i = 0; i < MAX_THREAD_B_; i++) {
        cv_b_.notify_all();                                    // "unlock" all the threads that waits on the condition variable
        thread_pool_b_[i].join();
    }

#ifdef APP_DEBUG
	std::cout << "Download manager stopped" << std::endl;
#endif 
}

bool DownloadManager::insert_big_file(const request_struct request, const connection_ptr connection) {

	download_struct dw_req;				// download request			
    dw_req.req = request;
    dw_req.conn = connection;

    std::unique_lock<std::mutex> ul(mtx_b_);
	const auto queue_insertion_res = big_file_q_.insert_element(dw_req);

    if (queue_insertion_res) {			// if the connection is insert correctly into the queue notify it to the threads and then return true otherwise return false; 
        cv_b_.notify_all();
        return true;
    }

    return false;
}

bool DownloadManager::insert_small_file(const request_struct request, const connection_ptr connection) {
	
    download_struct dw_req;		// download request 
	dw_req.req = request;
    dw_req.conn = connection;

    std::unique_lock<std::mutex> ul(mtx_s_);
	const auto queue_insertion_result = small_file_q_.insert_element(dw_req);

    if (queue_insertion_result) {
        cv_s_.notify_all();
        return true;
    }

    return false;
}

void DownloadManager::process_small_file(int thread_id) {

	int id = thread_id;

	// declare a new download struct 
	download_struct small_file_req;

    std::unique_lock<std::mutex> ul(mtx_s_, std::defer_lock);
	auto exit = false;

    while (!exit) {
        ul.lock();

        cv_s_.wait(ul, [this] {
            return (!small_file_q_.is_empty() && !is_terminated_.load());
        });

        // get the request struct from the queue than release the lock
        if (is_terminated_.load())
			exit = true;
        else 
			small_file_q_.pop_element(small_file_req);

        // release the queue lock
        ul.unlock();
        
		if (!exit)
			process_file(small_file_req, id);

		small_file_req.conn->close_connection();
    }
}

void DownloadManager::process_big_file(int thread_id) {

	int id = thread_id;

	download_struct big_file_req;

	std::unique_lock<std::mutex> ul(mtx_b_, std::defer_lock);
	auto exit = false;

    while (!exit) {
        ul.lock();

        cv_b_.wait(ul, [this] {
            return (!big_file_q_.is_empty() && !is_terminated_.load());
        });

		// check if the server is in terminated status
        if (is_terminated_.load()) 
			exit = true;
        else
			big_file_q_.pop_element(big_file_req);

		// release the lock 
        ul.unlock();
        
		if (!exit) 
			process_file(big_file_req, id);
      
        big_file_req.conn->close_connection(); 
    }
}

void DownloadManager::process_file(download_struct file_req, int thread_id) {
	try {
		TemporaryFile temporary_file;

		// download the file and store it into the temp folder
		if (!download_file(file_req, temporary_file)) {
			ConcurrentStreamPrint::print_data(thread_id, class_name, "impossible to complete the file download...");
			return;
		}

		std::string filename = file_req.req.file_name_;
		FileHandler destination_file(filename, path_);

		// copy the file into the destination file
		if (!copy_file(temporary_file, destination_file)) {
			destination_file.remove_file();
		} 

		std::stringstream ss;
		ss << filename << " downloaded correctly";

		ConcurrentStreamPrint::print_data(thread_id, class_name, ss.str());
	}
	catch (SocketException &se) {
		UNREFERENCED_PARAMETER(se);
		ConcurrentStreamPrint::print_data(thread_id, class_name, "Socket Exception");
	}
	catch (TimeoutException &te) {
		UNREFERENCED_PARAMETER(te);
		ConcurrentStreamPrint::print_data(thread_id, class_name, "Timeout Exception");
	}
	catch (FileOpenException &fwe) {
		UNREFERENCED_PARAMETER(fwe);
		ConcurrentStreamPrint::print_data(thread_id, class_name, "File Open Exception");
	}
	catch (FileWriteException & fe) {
		UNREFERENCED_PARAMETER(fe);
		ConcurrentStreamPrint::print_data(thread_id, class_name, "File Write Exception");
	}
}

bool DownloadManager::download_file(download_struct request,  TemporaryFile &temporary_file) {
	int left_bytes = request.conn->read_file(request.req.file_size_, temporary_file);
	return send_response(left_bytes, request);
}

bool DownloadManager::send_response(int left_bytes, download_struct request) {
	if (left_bytes == 0) {
		PacketDispatcher packet_fwd(request.conn);
		packet_fwd.send_packet(protocol::ok);
		return true;
	}

#ifdef APP_DEBUG
	std::cout << "connection closed, left bytes: " << left_bytes << std::endl;
#endif 

	return false;
}

bool DownloadManager::copy_file(TemporaryFile &temporary_file, FileHandler &destination_file) {
	// check write permission for destination file
	if (!destination_file.check_write_permission())
		return false;

	temporary_file.open_file(read);
    destination_file.open_file(write);

	//std::cout << temporary_file.get_filename() << std::endl;
	//std::cout << destination_file.get_filename() << std::endl;

	bool result;

    if (temporary_file.copy_file(destination_file)) {
		result = true;
	} else {
		result = false;
	}

	temporary_file.close_file();
	destination_file.close_file();
	return result;
}