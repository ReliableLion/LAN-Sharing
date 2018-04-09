#pragma once

#include <ctime>
#include <thread>
#include <mutex>

#include "Connection.hpp"
#include "RequestManager.hpp"

enum server_state_type
{
	CREATED,
	PAUSE,
	STOP,
	RUN
};


class Server
{
	std::thread server_main_thread_;

	Listen_socket socket_;
	std::shared_ptr<RequestManager> request_manager_;
	std::shared_ptr<DownloadManager> download_manager_;

	std::condition_variable cv_;
	std::condition_variable cv2_;
	std::mutex mtx_;
	std::mutex mtx2_;

	bool  is_paused_, is_stopped_;
	server_state_type server_status_;

	// private methods
	void listen_new_connection();
	void run_server();
public:
	Server();
	~Server();
	bool change_port(int port);
	void start_server();
	void restart_server();
	void pause_server();
	void rerun_server();
	void close_server();	
};
