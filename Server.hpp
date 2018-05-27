#pragma once

#include "stdafx.h"

#include <ctime>
#include <thread>
#include <mutex>

#include "Connection.hpp"
#include "RequestManager.hpp"

enum server_state_type {
	CREATED,
	PAUSED,
	STOPPED,
	RUNNING
};


class Server {

	SOCKET passive_socket_;
	SOCADDR_IN local_address_;

	std::thread server_main_thread_;

	std::shared_ptr<RequestManager> request_manager_;
	std::shared_ptr<DownloadManager> download_manager_;

	std::condition_variable cv_;
	std::mutex mtx_;

	bool  is_paused_, is_stopped_;
	server_state_type server_status_;

	void listen_new_connection();
	void run_server();
public:
	Server(int port);
	~Server();
	void start_server();
	void restart_server();
	void pause_server();
	void rerun_server();
	void close_server();
};
