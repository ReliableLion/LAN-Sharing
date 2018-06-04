#pragma once

#include "Constants.hpp"
#include "Connection.hpp"
#include "DownloadManager.hpp"
#include "ConcurrentQueue.hpp"
#include "PacketManager.hpp"
#include "Message.hpp"

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <vector>

enum request_status {
    full_queue = 0,
    term_signal
};

class RequestManager {
    // thread & synchronization variables
    const int MAX_THREADS_ = REQUEST_THREADS;
    const int FILE_THRESHOLD_ = FILE_SIZE_THRESHOLD;
    const int MAX_REQUEST_ATTEMPTS_ = MAX_REQUEST_ATTEMPTS;

    // synchronization variable decalration
    std::atomic<bool> is_terminated_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::vector<std::thread> thread_pool_;

    // connection and download variable
    ConcurrentQueue<connection::conn_ptr> connection_queue_;
    std::shared_ptr<DownloadManager> download_manager_;

    void extract_next_connection();

    void download_request(connection::conn_ptr connection) const;

	static bool validate_request(PacketManager &packet_manager, request_struct request);

    bool forward_request(request_struct request, connection::conn_ptr connection) const;
    //bool send_response(PacketManager& res_packet_manager, connection::conn_ptr connection);
public:
	explicit RequestManager(std::shared_ptr<DownloadManager> download_manager);

    ~RequestManager();

    void terminate_service();

    bool add_connection(connection::conn_ptr new_connection, request_status &status);
};
