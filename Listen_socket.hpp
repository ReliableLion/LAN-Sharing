#pragma once

#include "WinSock2.h"

#include <iostream>
#include <string>

#pragma comment(lib,"ws2_32.lib") //Required for WinSock

class Listen_socket {
	int server_port_;			// local server port 
	SOCKET l_socket_;			// listen socket
	SOCKADDR_IN local_addr_;		// local server address
public:
	Listen_socket(int port);
	Listen_socket(const Listen_socket& socket);
	Listen_socket& operator= (const Listen_socket& socket);
	~Listen_socket();
	SOCKET get_socket();
};

