#pragma once
#include "pch.h"

struct FileRequest {
    std::string file_name_;
	std::string requestID;
    user destination_user;
    DWORD file_size_;
    size_t transferred_bytes_ = 0;
    std::shared_ptr<connection::TcpConnection> connection_;
};

class UploadManager;

class RequestHandler {
private:
    // thread & synchronization variables
    const int MAX_THREADS_ = REQUEST_THREADS;
    const int MAX_REQUEST_ATTEMPTS_ = MAX_REQUEST_ATTEMPTS;

    // connection and download variable
    //std::list<FileRequest> requests_;
	std::multimap<user_request, std::future<bool>> requests_;

    // synchronization variable decalration
    std::atomic<bool> is_terminated_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::vector<std::thread> thread_pool_;

public:
    RequestHandler();

    //~request_handler();

	bool is_terminated(user_request destination_user);
	bool get_result(user_request destination_user);

    std::string send_request(user_request destination_user);
};
