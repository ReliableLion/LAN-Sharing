#pragma once

#include "Constants.hpp"
#include "Connection.hpp"
#include "DownloadManager.hpp"
#include "ConcurrentQueue.hpp"
#include "PacketManager.hpp"
#include "FileHandler.hpp"
#include "SocketException.h"
#include "TimeoutException.hpp"

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <vector>

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

	// synchronization variable decalration
	std::atomic<bool> is_terminated;
	std::mutex mtx;
	std::condition_variable cv;
	std::vector<std::thread> threadPool;
	
	// connection variables
	ConcurrentQueue<connection::conn_ptr> connectionQueue;

	std::shared_ptr<DownloadManager> dwload_manager;

	// private methods
	void extract_next_connection();
	void receiveRequest(connection::conn_ptr connection);
	bool processRequest(PacketManager& req_packet_manager,connection::conn_ptr connection);
	bool sendResponse(PacketManager& res_packet_manager, connection::conn_ptr connection);
public:
	RequestManager(std::shared_ptr<DownloadManager>);
	~RequestManager();
	void terminateService();
	bool addConnection(connection::conn_ptr newConnection, request_status& status);
};
