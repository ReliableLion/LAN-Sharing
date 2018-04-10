#pragma once
#include "Constants.hpp"
#include "UploadManager.hpp"
#include "Connection.hpp"

#include <memory>
#include "ConcurrentQueue.hpp"
#include <atomic>
#include <map>
#include "PacketManager.hpp"

struct file_request {
	std::string file_name;
	std::string destination_address;
	DWORD file_size;
	size_t transferred_bytes = 0;
	std::shared_ptr<connection::TCPConnection> connection;
};

class request_handler {

	// thread & synchronization variables
	const int max_threads_ = REQUEST_THREADS;
	const int max_request_attempts_ = MAX_REQUEST_ATTEMPTS;

	// connection and download variable 
	std::list<file_request> requests_;
	std::shared_ptr<upload_manager> upload_manager_;
	PacketManager packet_manager_;

	// synchronization variable decalration
	std::atomic<bool> is_terminated_;
	std::mutex mtx_;
	std::condition_variable cv_;
	std::vector<std::thread> thread_pool_;

	static std::string request_handler::get_file_name_from_full_path(const std::string& file_path);

public:
	request_handler(std::shared_ptr<upload_manager>);
	~request_handler();
	bool send_request(char* server, char *file_path);
	
};
