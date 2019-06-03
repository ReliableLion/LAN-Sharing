#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <vector>

#include "Constants.hpp"
#include "Connection.hpp"
#include "DownloadManager.hpp"
#include "ConcurrentQueue.hpp"
#include "PacketDispatcher.hpp"
#include "Message.hpp"

enum request_status {
    full_queue = 0,
    term_signal
};

typedef struct QueueConnectionElement {
	connection::connection_ptr connection;
	int attempts;
} QueueConnectionElement;

class HandshakeManager {
    // thread & synchronization variables
    const int MAX_THREADS_ = REQUEST_THREADS;
    const int FILE_THRESHOLD_ = FILE_SIZE_THRESHOLD;
    const int MAX_REQUEST_ATTEMPTS_ = MAX_REQUEST_ATTEMPTS;

    // synchronization variable declation
    std::atomic<bool> is_terminated_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::vector<std::thread> thread_pool_;

	const std::string class_name = "Handshake Manager";

    // connection and download variable
    ConcurrentQueue<QueueConnectionElement> connection_queue_;
    std::shared_ptr<DownloadManager> download_manager_;

	// this private method extract incoming connections from queue 
    void extract_connections_from_queue(int id);

	void process_client_req(PacketDispatcher &packet_dispacher, connection::connection_ptr connection, int thread_id) const;

	void append_in_download_queue(PacketDispatcher &packet_dispatcher, const request_struct request, connection::connection_ptr connection) const;

	void requeue_connection(QueueConnectionElement element, int thread_id, std::string err_msg);

	// bool validate_request(PacketDispatcher &packet_fwd, request_struct request);

	// void decode_send_request();

    // bool send_response(PacketManager& res_packet_manager, connection::conn_ptr connection);
public:
	explicit HandshakeManager(std::shared_ptr<DownloadManager> download_manager);

    ~HandshakeManager();

    void terminate_service();

    bool add_connection(QueueConnectionElement element, request_status &status);
};
