#pragma once
#include "stdafx.h"
#include "Session.hpp"
#include "ConcurrentQueue.hpp"
#include <mutex>
#include <atomic>
#include <thread>
#include <vector>

class RequestManager {
private:
	ConcurrentQueue<session::conn_ptr> connectionQueue;
	std::atomic<bool> is_terminated;
	const int maxThreads = 4;
	std::vector<std::thread> threadPool;
private:
	void extractConnection();
	void processRequest();
public:
	RequestManager();
	~RequestManager();
	bool RequestManager::addConnection(session::conn_ptr newConnection);
};
