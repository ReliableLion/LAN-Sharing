#pragma once
#include "stdafx.h"
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
	ConcurrentQueue<session::conn_ptr> connectionQueue;
	std::atomic<bool> is_terminated;
	const int maxThreads = 4;
	const int maxRequestAttempts = 3;
	std::mutex mtx;
	std::condition_variable cv;
	std::vector<std::thread> threadPool;
private:
	void extractConnection();
	void processRequest();
public:
	RequestManager();
	~RequestManager();
	bool RequestManager::addConnection(session::conn_ptr newConnection);
};
