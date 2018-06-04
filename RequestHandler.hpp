#pragma once

#include "stdafx.h"

#include <memory>
#include <atomic>

#include "Constants.hpp"
#include "UploadManager.hpp"
#include "Connection.hpp"
#include "ConcurrentQueue.hpp"
#include "PacketManager.hpp"

struct FileRequest {
    std::string file_name_;
    std::string destination_address_;
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
    std::list<FileRequest> requests_;
    std::shared_ptr<UploadManager> upload_manager_;

    // synchronization variable decalration
    std::atomic<bool> is_terminated_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::vector<std::thread> thread_pool_;

public:
    RequestHandler(const std::shared_ptr<UploadManager> upload_manager);

    //~request_handler();
    bool send_request(std::string server, char *file_path);
};
