#pragma once
#include "stdafx.h"
#include "Session.hpp"
#include "RequestManager.hpp"
#include "Constants.hpp"
#include <ctime>

//
class Server {
	/*const int default_port = DEFAULT_LISTEN_PORT;
	SOCKET l_socket;			// listen socket
	SOCKADDR_IN local_addr;		// local server address*/
	// TODO request manager instance
	// TODO download manager isntance
	Listen_socket socket;
	RequestManager request_manager;
	
public:
	Server();
	~Server();
	bool changePort(int port);
	void listenNewConnection();
	void closeServer();					// TODO check if this method in necessary or not
};
