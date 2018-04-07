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
	const int max_thread_b_ = BIG_FILE_THREADS;
	const int max_thread_s_ = SMALL_FILE_THREADS;
	std::vector<std::thread> thread_pool_b_;
	std::vector<std::thread> thread_pool_s_;
	ConcurrentQueue<download_struct> big_file_q_;
	ConcurrentQueue<download_struct> small_file_q_;
	std::mutex mtx_b_;
	std::mutex mtx_s_;
	std::atomic<bool> is_terminated_;
	std::condition_variable cv_b_, cv_s_;

	// file variables
	std::string path_;
	const std::string temp_path_ = TEMP_PATH;

	// private methods
	void process_big_file();
	void process_small_file();
	//void manageDownload(download_struct req_struct);
	void download_file(download_struct request);
public:
	DownloadManager();
	~DownloadManager();
	void terminate_service();
	bool insert_small_file(request_struct request, connection::conn_ptr connection);
	bool insert_big_file(request_struct request, connection::conn_ptr connection);
};