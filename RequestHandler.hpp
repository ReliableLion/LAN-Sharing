#pragma once
#include "pch.h"

struct FileRequest {
    std::string file_name_;
	std::string requestID;
    user destination_user;
    DWORD file_size_;
    size_t transferred_bytes_ = 0;
    std::shared_ptr<connection::TcpConnection> connection_;
	bool directory;
};

class UploadManager;

class RequestHandler {
private:
    // thread & synchronization variables
    const int MAX_THREADS_ = REQUEST_THREADS;
    const int MAX_REQUEST_ATTEMPTS_ = MAX_REQUEST_ATTEMPTS;

    // connection and download variable
    //std::list<FileRequest> requests_;
	std::map<string, std::future<bool>> requests_;
	std::map<string, shared_ptr<FileRequest>> fileRequests_;

    // synchronization variable decalration
    std::atomic<bool> is_terminated_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::vector<std::thread> thread_pool_;

public:
    RequestHandler();

    //~request_handler();

	bool is_terminated(string id);
	bool get_result(string id);

    bool send_request(user_request destination_user, char* requestIDBuff, bool directory);
	void cancel_request(string id);
};
