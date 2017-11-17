#pragma once
#include "stdafx.h"
#include "ConcurrentQueue.hpp"
#include "FileHandler.hpp"
#include "Session.hpp"
#include "Message.hpp"
#include "windows.h"
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>


typedef struct {
	requestStruct req;
	session::conn_ptr conn;
} dw_request;

class DownloadManager {
private:
	// thread & synchronization variables
	const int maxThreadB = BIG_FILE_THREADS , maxThreadS = SMALL_FILE_THREADS;
	std::vector<std::thread> threadPoolB, threadPoolS;
	ConcurrentQueue<dw_request> bigFileQ, smallFileQ;
	std::mutex mtxB, mtxS;
	std::atomic<bool> is_terminated;
	std::condition_variable cvB, cvS;

	// file variables
	std::string path;
	const std::string temp_path = TEMP_PATH;
private:
	void processBigFile();
	void processSmallFile();
	void downloadFile(dw_request request);
public:
	DownloadManager();
	~DownloadManager();
	bool insertSmallFile(__int64 size, FILETIME timestamp, std::string fileName, session::conn_ptr connection);
	bool insertBigFile(__int64 size, FILETIME timestamp, std::string fileName, session::conn_ptr connection);
};