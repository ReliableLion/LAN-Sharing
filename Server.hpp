#pragma once

#include <thread>
#include <mutex>

#include "Connection.hpp"
#include "HandshakeManager.hpp"

enum server_state_type {
    stopped,
    running
};

class Server {

    SOCKET passive_socket_;
    SOCKADDR_IN local_address_;

    std::thread server_main_thread_;

    server_state_type server_status_;

	const std::string class_name = "Server";

    std::shared_ptr<HandshakeManager> handshake_agreement_manager_;
    std::shared_ptr<DownloadManager> download_manager_;

    void listen_new_connection();

    void run_server();

	static void print_client_info(std::chrono::time_point<std::chrono::system_clock> time_stamp,
							std::shared_ptr<connection::TcpConnection> connection);

public:
	explicit Server(int port = DEFAULT_LISTEN_PORT);

    ~Server();

    void start_server();

    void close_server();
};
