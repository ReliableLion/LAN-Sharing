#include "DownloadManager.hpp"

DownloadManager::DownloadManager() {
	is_terminated_.store(false);

	for (int i = 0; i < max_thread_b_; i++) {												// threads declarations for the big file download
		thread_pool_b_.push_back(std::thread(&DownloadManager::process_big_file, this));		// pass to these threads the process big file methods
	}

	for (int i = 0; i < max_thread_s_; i++) {												// threads declarations for the small file download
		thread_pool_s_.push_back(std::thread(&DownloadManager::process_small_file, this));	// pass to these threads the process small file methods
	}
}

DownloadManager::~DownloadManager() {
	terminate_service();
}

void DownloadManager::terminate_service() {
	is_terminated_.store(true);								// set is terminated true in order to stop the threds execution

	for (int i = 0; i < max_thread_s_; i++) {	// join all the threads
		cv_s_.notify_all();									// "unlock" all the threads that waits on the condition variable
		thread_pool_s_[i].join();
	}

	for (int i = 0; i < max_thread_b_; i++) {	// join all the threads
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
bool DownloadManager::insert_big_file(request_struct request, connection::conn_ptr connection) {
	download_struct newRequest;
	bool queue_insertion_res;

	// fill the request struct with the date received by the client
	newRequest.req = request;
	newRequest.conn = connection;

	std::unique_lock<std::mutex> ul(mtx_b_);
	queue_insertion_res = big_file_q_.insertElement(newRequest);

	if (queue_insertion_res)										// if the connection is insert correctly into the queue notify it to the threads and then return true otherwise return false;
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

//				PRIVATE METHODS	

void DownloadManager::process_small_file() {
	std::unique_lock<std::mutex> ul(mtx_s_, std::defer_lock);
	download_struct smallFileReq;
	bool exit = false;

	while (exit) {
		ul.lock();

		cv_s_.wait(ul, [this] {
			return (!small_file_q_.isEmpty() && !is_terminated_.load());
		});

		if (is_terminated_.load()) exit = true;
		small_file_q_.popElement(smallFileReq);

		// TODO insert here the function for the file download
		download_file(smallFileReq);
		smallFileReq.conn->close_connection();
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

		if (is_terminated_.load()) exit = true;									// if receive a notify to stop the downlaod exit from the main loop
		small_file_q_.popElement(bigFileReq);

		// check if the connection os alive
		// if not go to the next pending resource
		// TODO insert here the function for the file download
		download_file(bigFileReq);

	}
}

void DownloadManager::download_file(download_struct request) {
	//FileHandler dest_file(request.req.file_name, path);
	TemporaryFile temp_file(request.req.file_name, temp_path_);
	int leftByte = request.req.file_size;
	int bytes_to_downlaod = 0, downloaded_bytes = 0;
	char buffer[MAXBUFL];
	bool connection_closed;

	try {
		temp_file.openFile(WRITE);								// open the two files, if an exception is throw by the program then the file is closed by the destructor

		while (leftByte != 0 && !connection_closed) {
			if (leftByte >= MAXBUFL)					// if the remaining data are greater than the max size of the buffer then the bytes to download are max buff lenght
				bytes_to_downlaod = MAXBUFL;
			else
				bytes_to_downlaod = leftByte;				// if the remaining data are smaller than the max, set the remaining bytes value

			if (request.conn->recv_all(buffer, bytes_to_downlaod, downloaded_bytes))			// check if the connection is 
			{
				leftByte -= downloaded_bytes;
				temp_file.writeData(buffer, downloaded_bytes);
			}
			else
				connection_closed = true;
		}


		FileHandler destination_file(request.req.file_name, path_);
		destination_file.openFile(WRITE);

		if (destination_file.copyFile(temp_file)) {
			temp_file.closeFile();
			destination_file.closeFile();
		}
		else {
			// TODO is not possible to copy the temp file into the destination file
		}

		request.conn->close_connection();
	}
	catch (SocketException &se) {
		std::cout << "server error: " << se.what() << std::endl;
		request.conn->close_connection();
	}
	catch (FileWriteException &fwe) {
		std::cout << "impossible to write the data into the specified file" << std::endl;
		request.conn->close_connection();
	}
	catch (TimeoutException &te) {
		std::cout << "connection reached timeout, closing the connection" << std::endl;
		request.conn->close_connection();
	}
}