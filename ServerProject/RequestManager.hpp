#pragma once
#include "stdafx.h"
#include "Constant.hpp"
#include "Session.hpp"
#include "ConcurrentQueue.hpp"
#include "FileHandler.hpp"
#include "SocketException.h"
#include "TimeoutException.hpp"
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <vector>

class RequestManager {
private:
	// thread & synchronization variables
	const int maxThreads = REQUEST_THREADS;
	const int fileThreshold = FILE_SIZE_THRESHOLD;
	const int maxRequestAttempts = MAX_REQUEST_ATTEMPTS;
	std::atomic<bool> is_terminated;
	std::mutex mtx;
	std::condition_variable cv;
	std::vector<std::thread> threadPool;
	
	// connection variables
	ConcurrentQueue<session::conn_ptr> connectionQueue;
private:
	// private methods
	void extractConnection();
	void processRequest();
	bool sendReply(session::conn_ptr conn);
public:
	RequestManager();
	~RequestManager();
	bool RequestManager::addConnection(session::conn_ptr newConnection);
};
