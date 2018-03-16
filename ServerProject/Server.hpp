#pragma once
#include "stdafx.h"
#include "Connection.hpp"
#include "RequestManager.hpp"
#include "Constants.hpp"
#include <ctime>

enum server_state_type
{
	CREATED,
	STOPPED,
	RUNNING
};


class Server {
	// TODO request manager instance
	// TODO download manager isntance


	server_state_type server_state;

	Listen_socket socket;
	std::shared_ptr<RequestManager> request_manager;
	std::shared_ptr<DownloadManager> download_manager;
	
public:
	Server();
	~Server();
	bool changePort(int port);
	void listenNewConnection();
	void runServer();
	void closeServer();					// TODO check if this method in necessary or not
};
