#pragma once
#include "stdafx.h"
#include "Constants.hpp"
#include "Session.hpp"
#include "DownloadManager.hpp"
#include "ConcurrentQueue.hpp"
#include "FileHandler.hpp"
#include "SocketException.h"
#include "TimeoutException.hpp"
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <vector>
#include "PacketManager.hpp"

enum request_status
{
	FULL_QUEUE = 0,
	TERM_SIGNAL
};

class RequestManager {
	// thread & synchronization variables
	const int maxThreads = REQUEST_THREADS;
	const int fileThreshold = FILE_SIZE_THRESHOLD;
	const int max_request_attempts = MAX_REQUEST_ATTEMPTS;
	std::atomic<bool> is_terminated;
	std::mutex mtx;
	std::condition_variable cv;
	std::vector<std::thread> threadPool;
	
	// connection variables
	ConcurrentQueue<session::conn_ptr> connectionQueue;

	// download manager for the file download
	DownloadManager download_manager;
	// private methods
	void extractConnection();
	void receiveRequest(session::conn_ptr connection);
	bool processRequest(PacketManager& packet_manager, session::conn_ptr connection);
	bool sendReply(session::conn_ptr connection);
public:
	RequestManager();
	~RequestManager();
	bool addConnection(session::conn_ptr newConnection, request_status& status);
};
