#pragma once

#include "stdafx.h"

#include <ctime>
#include <thread>
#include <mutex>

#include "Connection.hpp"
#include "RequestManager.hpp"

enum server_state_type
{
	CREATED,
	PAUSED,
	STOPPED,
	RUNNING
};


class Server
{
	std::thread server_main_thread;

	Listen_socket socket;
	std::shared_ptr<RequestManager> request_manager;
	std::shared_ptr<DownloadManager> download_manager;

	std::condition_variable cv;
	std::mutex mtx;

	bool  isPaused, isStopped;
	server_state_type server_status;

	// private methods
	void listenNewConnection();
	void runServer();
public:
	Server();
	~Server();
	bool changePort(int port);
	void startServer();
	void restartServer();
	void pauseServer();
	void rerunServer();
	void closeServer();	
};
