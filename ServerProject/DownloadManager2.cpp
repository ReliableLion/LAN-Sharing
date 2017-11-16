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
}

//				PRIVATE METHODS	

