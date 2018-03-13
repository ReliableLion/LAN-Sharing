#pragma once
#include <WinSock2.h>
#include "Constants.hpp"

class Listen_socket
{
	int server_port;			// local server port 
	SOCKET l_socket;			// listen socket
	SOCKADDR_IN local_addr;		// local server address
public:
	Listen_socket(int port);
	Listen_socket(const Listen_socket& socket);
	Listen_socket& operator= (const Listen_socket& socket);
	~Listen_socket();
	SOCKET getSocket();
};

