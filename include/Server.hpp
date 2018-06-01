#pragma once

#include "stdafx.h"

#include <thread>
#include <mutex>
#include <ctime>

#include "Connection.hpp"
#include "RequestManager.hpp"

enum server_state_type {
	STOPPED,
	RUNNING
};


class Server {

	SOCKET passive_socket_;
	SOCKADDR_IN local_address_;

	std::thread server_main_thread_;

	server_state_type server_status;

	std::shared_ptr<RequestManager> request_manager_;
	std::shared_ptr<DownloadManager> download_manager_;

	void listen_new_connection();
	void run_server();
	void print_client_info(std::chrono::time_point<std::chrono::system_clock> time_stamp, connection::TCPConnection &connection);
public:
	Server(int port);
	~Server();
	void start_server();
	void close_server();
};
