#pragma once

#include "Constants.hpp"
#include "Connection.hpp"
#include "DownloadManager.hpp"
#include "ConcurrentQueue.hpp"
#include "PacketManager.hpp"
#include "FileHandler.hpp"
#include "Exceptions.hpp"
#include "PacketManager.hpp"

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <vector>

enum request_status {
	FULL_QUEUE = 0,
	TERM_SIGNAL
};

class RequestManager {
	// thread & synchronization variables
	const int max_threads_ = REQUEST_THREADS;
	const int file_threshold_ = FILE_SIZE_THRESHOLD;
	const int max_request_attempts_ = MAX_REQUEST_ATTEMPTS;

	// synchronization variable decalration
	std::atomic<bool> is_terminated_;
	std::mutex mtx_;
	std::condition_variable cv_;
	std::vector<std::thread> thread_pool_;

	// connection and download variable 
	ConcurrentQueue<connection::conn_ptr> connection_queue_;
	std::shared_ptr<DownloadManager> download_manager;

	void extract_next_connection();

	void download_request(connection::conn_ptr connection);
	bool validate_request(PacketManager &packet_manager, request_struct request);
	bool forward_request(request_struct request, connection::conn_ptr connection);
	//bool send_response(PacketManager& res_packet_manager, connection::conn_ptr connection);
public:
	RequestManager(std::shared_ptr<DownloadManager> download_manager);
	~RequestManager();
	void terminate_service();
	bool add_connection(connection::conn_ptr new_connection, request_status& status);
};
