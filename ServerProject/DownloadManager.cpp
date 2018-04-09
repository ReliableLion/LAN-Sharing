#include "stdafx.h"

#include "DownloadManager.hpp"

DownloadManager::DownloadManager() 
{
	is_terminated_.store(false);

	// threads declarations for the big file download
	for (int i = 0; i < max_thread_b_; i++)									
		thread_pool_b_.push_back(std::thread(&DownloadManager::process_big_file, this));		// pass to these threads the process big file methods

	// threads declarations for the small file download
	for (int i = 0; i < max_thread_s_; i++)													
		thread_pool_s_.push_back(std::thread(&DownloadManager::process_small_file, this));		// pass to these threads the process small file methods
}

DownloadManager::~DownloadManager()
{
	terminate_service();
}

void DownloadManager::terminate_service()
{
	is_terminated_.store(true);								// set is terminated true in order to stop the threds execution

	for (int i = 0; i < max_thread_s_; i++)
	{	
		// join all the threads
		cv_s_.notify_all();									// "unlock" all the threads that waits on the condition variable
		thread_pool_s_[i].join();
	}

	for (int i = 0; i < max_thread_b_; i++) 
	{	
		// join all the threads
		cv_b_.notify_all();									// "unlock" all the threads that waits on the condition variable
		thread_pool_b_[i].join();
	}
}


/**
 * \brief 
 * \param request this is the request struct that is used for the data download
 * \param connection pointer to the TCP connection
 * \return TRUE if the file is insert correctly, FALSE if the queue is full of possible elemnts
 */
bool DownloadManager::insert_big_file(request_struct request, connection::conn_ptr connection)
{
	download_struct newRequest;
	
	// fill the request struct with the date received by the client
	newRequest.req = request;
	newRequest.conn = connection;

	std::unique_lock<std::mutex> ul(mtx_b_);
	bool queue_insertion_result = big_file_q_.insert_element(newRequest);

	if(queue_insertion_result)										// if the connection is insert correctly into the queue notify it to the threads and then return true otherwise return false;
	{
		cv_b_.notify_all();
		return true;
	}
	
	return false;
}

/**
 * \brief 
 * \param request : struct that contains the informations about the file that the client requests
 * \param connection : connection 
 * \return 
 */
bool DownloadManager::insert_small_file(request_struct request, connection::conn_ptr connection)
{
	download_struct new_request;
	
	new_request.req = request;
	new_request.conn = connection;

	std::unique_lock<std::mutex> ul(mtx_s_);
	bool queue_insertion_result = small_file_q_.insert_element(new_request);

	if(queue_insertion_result)
	{
		cv_s_.notify_all();
		return true;
	}

	return false;
}

//				PRIVATE METHODS	

/*
 *  TODO process small file and process big file must take care of the entire downlaod of the file
 *  this class has an entry point of communication with the user interface
 */

void DownloadManager::process_small_file() {
	std::unique_lock<std::mutex> ul(mtx_s_, std::defer_lock);
	download_struct smallFileReq;
	bool exit = false;

	while (exit)
	{
		ul.lock();

		cv_s_.wait(ul, [this] {
			return (!small_file_q_.is_empty() && !is_terminated_.load());
		});

		if (is_terminated_.load()) exit = true;
		small_file_q_.pop_element(smallFileReq);

		download_file(smallFileReq);
	}
}

void DownloadManager::process_big_file()
{
	std::unique_lock<std::mutex> ul(mtx_b_, std::defer_lock);
	download_struct big_file_req;
	bool exit = false;

	while (exit)
	{
		ul.lock();

		cv_b_.wait(ul, [this] {
			return (!big_file_q_.is_empty() && !is_terminated_.load());
		});

		if (is_terminated_.load()) exit = true;									// if receive a notify to stop the downlaod exit from the main loop
		small_file_q_.pop_element(big_file_req);

		// check if the connection os alive
		// if not go to the next pending resource

		download_file(big_file_req);
	}
}


/*
 *  return a status code that that is forwarder to the user inteface
 */
int DownloadManager::download_file(download_struct request)
{
	TemporaryFile temp_file(request.req.file_name, temp_path_);
	int left_bytes = request.req.file_size;
	int bytes_to_download = 0, downloaded_bytes = 0;
	char buffer[BUFF_LENGTH];
	bool connection_closed = false;

	try 
	{
		temp_file.open_file(WRITE);								// open the two files, if an exception is throw by the program then the file is closed by the destructor

		// download the file 
		while (left_bytes != 0 && !connection_closed)
		{
			if (left_bytes >= BUFF_LENGTH)					// if the remaining data are greater than the max size of the buffer then the bytes to download are max buff length
				bytes_to_download = BUFF_LENGTH;
			else
				bytes_to_download = left_bytes;				// if the remaining data are smaller than the max, set the remaining bytes value

			if (request.conn->recv_all(buffer, bytes_to_download, downloaded_bytes))			// check if the connection is 
			{
				left_bytes -= downloaded_bytes;
				temp_file.write_data(buffer, downloaded_bytes);
			}
			else
				connection_closed = true;
		}

		// if the file is not complete return an error code
		if(left_bytes != 0)
		{
			temp_file.remove_file();
			return FILE_NOT_COMPLETE;
		}

		FileHandler destination_file(request.req.file_name, path_);
		destination_file.open_file(WRITE);

		if (destination_file.copy_file(temp_file))
		{
			temp_file.close_file();
			destination_file.close_file();
		}
		else
		{
			// TODO is not possible to copy the temp file into the destination file
			std::cout << "is not possible to copy the temp file into the destination file" << std::endl;
			return COPY_ERROR;
		}

		request.conn->close_connection();
		return OK;
	}
	catch (SocketException &se)
	{
		std::cout << "server error: " << se.what() << std::endl;
		request.conn->close_connection();
		return SOCKET_ERROR_2;
	}
	catch (FileWriteException &fwe)
	{
		std::cout << "impossible to write the data into the specified file" << std::endl;
		request.conn->close_connection();
		return FILE_WRITE_ERROR;
	}
	catch (TimeoutException &te)
	{
		std::cout << "connection reached timeout, closing the connection" << std::endl;
		request.conn->close_connection();
		return TIMEOUT_ERROR;
	}
	
}