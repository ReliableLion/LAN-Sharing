#pragma once
#include "stdafx.h"
#include "Session.hpp"
#include <ctime>

//
class Server {
private:
	SOCKET l_socket;			// listen socket
	SOCKADDR_IN local_addr;		// local server address
	// TODO request manager instance
	// TODO download manager isntance
private: // private methods

public:
	Server(int port = 1500);
	~Server();
	void listenNewConnection();
	void closeServer();					// TODO check if this method in necessary or not
};
