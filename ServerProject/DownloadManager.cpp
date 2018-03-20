#include "DownloadManager.hpp"

DownloadManager::DownloadManager() {
	is_terminated.store(false);

	for (int i = 0; i < maxThreadB; i++) {												// threads declarations for the big file download
		threadPoolB.push_back(std::thread(&DownloadManager::processBigFile, this));		// pass to these threads the process big file methods
	}

	for (int i = 0; i < maxThreadS; i++) {												// threads declarations for the small file download
		threadPoolS.push_back(std::thread(&DownloadManager::processSmallFile, this));	// pass to these threads the process small file methods
	}
}

DownloadManager::~DownloadManager() {
	terminateService();
}

void DownloadManager::terminateService() {
	is_terminated.store(true);								// set is terminated true in order to stop the threds execution

	for (int i = 0; i < maxThreadS; i++) {					// join all the threads
		cvS.notify_all();									// "unlock" all the threads that waits on the condition variable
		threadPoolS[i].join();
	}

	for (int i = 0; i < maxThreadB; i++) {					// join all the threads
		cvB.notify_all();									// "unlock" all the threads that waits on the condition variable
		threadPoolB[i].join();
	}
}


/**
 * \brief 
 * \param request this is the request struct that is used for the data download
 * \param connection pointer to the TCP connection
 * \return TRUE if the file is insert correctly, FALSE if the queue is full of possible elemnts
 */
bool DownloadManager::insertBigFile(request_struct request, connection::conn_ptr connection) {
	download_struct newRequest;
	bool queue_insertion_res;

	// fill the request struct with the date received by the client
	newRequest.req = request;
	newRequest.conn = connection;

	std::unique_lock<std::mutex> ul(mtxB);
	queue_insertion_res = bigFileQ.insertElement(newRequest);

	if(queue_insertion_res)										// if the connection is insert correctly into the queue notify it to the threads and then return true otherwise return false;
	{
		cvB.notify_all();
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
bool DownloadManager::insertSmallFile(request_struct request, connection::conn_ptr connection) {
	download_struct newRequest;
	bool queue_insertion_res;


	newRequest.req = request;
	newRequest.conn = connection;

	std::unique_lock<std::mutex> ul(mtxS);
	queue_insertion_res = smallFileQ.insertElement(newRequest);

	if(queue_insertion_res)
	{
		cvS.notify_all();
		return true;
	}

	return false;
}

//				PRIVATE METHODS	

void DownloadManager::processSmallFile() {
	std::unique_lock<std::mutex> ul(mtxS, std::defer_lock);
	download_struct smallFileReq;

	while (true) {
		ul.lock();

		cvS.wait(ul, [this] {
			return (!smallFileQ.isEmpty() || is_terminated.load());
		});

		if (is_terminated.load()) break;
		smallFileQ.popElement(smallFileReq);

		// TODO insert here the function for the file download
		downloadFile(smallFileReq);
		smallFileReq.conn->close_connection();
	}
}

void DownloadManager::processBigFile() {
	std::unique_lock<std::mutex> ul(mtxB, std::defer_lock);
	download_struct bigFileReq;

	while (true) {
		ul.lock();

		cvB.wait(ul, [this] {
			return (!bigFileQ.isEmpty() || is_terminated.load());
		});

		if (is_terminated.load()) break;									// if receive a notify to stop the downlaod exit from the main loop
		smallFileQ.popElement(bigFileReq);

		// check if the connection os alive
		// if not go to the next pending resource



		// TODO insert here the function for the file download
		downloadFile(bigFileReq);
		bigFileReq.conn->close_connection();
	}
}

void DownloadManager::downloadFile(download_struct request) {
	OutputFileHandler dest_file(request.req.file_name, path);
	OutputFileHandler temp_file(request.req.file_name, temp_path);
	int leftByte = request.req.file_size;
	int bytes_to_downlaod = 0, downloaded_bytes = 0;
	char buffer[BUFF_LENGTH];

	try {
		temp_file.openFile();								// open the two files, if an exception is throw by the program then the file is closed by the destructor

		while (leftByte != 0) {

			if (leftByte >= BUFF_LENGTH) {					// if the remaining data are greater than the max size of the buffer then the bytes to download are max buff lenght
				bytes_to_downlaod = BUFF_LENGTH;
			}
			else {
				bytes_to_downlaod = leftByte;				// if the remaining data are smaller than the max, set the remaining bytes value
			}

			if (request.conn->recvall(buffer, bytes_to_downlaod, downloaded_bytes))			// check if the connection is 
			{
				leftByte -= downloaded_bytes;
				temp_file.writeData(buffer, downloaded_bytes);
			}
			else
			{
				break;
			}
		}

		request.conn->close_connection();					// close the connection 

		dest_file.openFile();
		if (dest_file.copyFile(temp_file)) {
			temp_file.closeFile();
			temp_file.removeFile();
			dest_file.closeFile();
		}
		else {
			// TODO is not possible to copy the temp file into the destination file
		}
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