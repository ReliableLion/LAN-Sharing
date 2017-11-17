#include "DownloadManager.hpp"

DownloadManager::DownloadManager() {
	is_terminated.store(false);

	for (int i = 0; i < maxThreadB; i++) {					// threads declarations for the big file download
		threadPoolB.push_back(std::thread());
	}

	for (int i = 0; i < maxThreadS; i++) {					// threads declarations for the small file download
		threadPoolS.push_back(std::thread());
	}
}

DownloadManager::~DownloadManager() {
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

bool DownloadManager::insertBigFile(__int64 size, FILETIME timestamp, std::string fileName, session::conn_ptr connection) {
	dw_request newRequest;
	newRequest.req.fileSize = size;
	newRequest.req.fileTimestamp = timestamp;
	newRequest.req.fileName = fileName;
	newRequest.conn = connection;

	std::unique_lock<std::mutex> ul(mtxB);
	bigFileQ.insertElement(newRequest);
	cvB.notify_all();
	return true;
}

bool DownloadManager::insertSmallFile(__int64 size, FILETIME timestamp, std::string fileName, session::conn_ptr connection) {
	dw_request newRequest;
	newRequest.req.fileName = size;
	newRequest.req.fileTimestamp = timestamp;
	newRequest.req.fileName = fileName;
	newRequest.conn = connection;

	std::unique_lock<std::mutex> ul(mtxS);
	smallFileQ.insertElement(newRequest);
	cvS.notify_all();
	return true;
}

//				PRIVATE METHODS	

void DownloadManager::processSmallFile() {
	std::unique_lock<std::mutex> ul(mtxS, std::defer_lock);
	dw_request smallFileReq;

	while (1) {
		ul.lock();

		cvS.wait(ul, [this] {
			return (!smallFileQ.isEmpty() || is_terminated.load());
		});

		if (!is_terminated.load()) break;
		smallFileQ.popElement(smallFileReq);

		// TODO insert here the function for the file download
	}
}

void DownloadManager::processBigFile() {
	std::unique_lock<std::mutex> ul(mtxB, std::defer_lock);
	dw_request bigFileReq;

	while (1) {
		ul.lock();

		cvB.wait(ul, [this] {
			return (!bigFileQ.isEmpty() || is_terminated.load());
		});

		if (!is_terminated.load()) break;
		smallFileQ.popElement(bigFileReq);

		// TODO insert here the function for the file download
	}
}

void DownloadManager::downloadFile(dw_request request) {
	OutputFileHandler dest_file(request.req.fileName, path);
	OutputFileHandler temp_file(request.req.fileName, temp_path);
	int leftByte = request.req.fileSize;
	
	try {
		dest_file.openFile();								// open the two files, if an exception is throw by the program then the file is closed by the destructor
		temp_file.openFile();

		while (leftByte != 0) {
			// TODO	put here the code for the file download
			//int readByte = request.conn->recvall()
		}
	}
	catch (SocketException &se) {
	}
	catch (FileWriteException &fwe) {

	}
}