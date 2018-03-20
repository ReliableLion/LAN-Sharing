#pragma once

#include "stdafx.h"

#include <windows.h>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "ConcurrentQueue.hpp"
#include "FileHandler.hpp"
#include "Connection.hpp"
#include "Message.hpp"

typedef struct {
	request_struct req;
	connection::conn_ptr conn;
} download_struct;

class DownloadManager {
	// thread & synchronization variables
	const int maxThreadB = BIG_FILE_THREADS , maxThreadS = SMALL_FILE_THREADS;
	std::vector<std::thread> threadPoolB, threadPoolS;
	ConcurrentQueue<download_struct> bigFileQ, smallFileQ;
	std::mutex mtxB, mtxS;
	std::atomic<bool> is_terminated;
	std::condition_variable cvB, cvS;

	// file variables
	std::string path;
	const std::string temp_path = TEMP_PATH;

	// private methods
	void processBigFile();
	void processSmallFile();
	void manageDownload(download_struct req_struct);
	void downloadFile(download_struct request);
public:
	DownloadManager();
	~DownloadManager();
	void terminateService();
	bool insertSmallFile(request_struct request, connection::conn_ptr connection);
	bool insertBigFile(request_struct request, connection::conn_ptr connection);
};