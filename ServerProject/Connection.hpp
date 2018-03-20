#pragma once

#include "stdafx.h"

#include <Ws2tcpip.h> 
#include <iostream>
#include <memory>
#include <iomanip>
#include <ctime>
#include <list>

#include "Listen_socket.h"
#include "SocketException.h"
#include "TimeoutException.hpp"

#pragma comment(lib,"ws2_32.lib") //Required for WinSock

namespace connection {

	class TCPConnection {
		// definition of the variable
		SOCKET sock = 0;
		SOCKADDR_IN remote_address;
		const long sec = TIMEOUT_SEC;
		const long usec = TIMEOUT_USEC;

		// definition of the private methods
		size_t readline_unbuffered(char *vptr, size_t maxlen);
	public:
		TCPConnection();
		bool accept_connection(Listen_socket s);
		bool close_connection() const;
		void print_endpoint_info() const;
		bool recvall(char *data, const int totalBytes, int& totalReadBytes);
		bool sendall(const char *data, const int totalBytes, int& totalSentBytes);
		bool readline(char *data, const int maxBytes, int& readBytes);
	};

	// definition of the connection pointer type 
	typedef std::shared_ptr<TCPConnection> conn_ptr;

	class Session
	{
		std::list<conn_ptr> connection_pool;

		// the constructor of copy and the assign operator are deleted
		Session(const Session& source) {}
		Session& operator= (const Session& source) {}
	public:
		Session();
		~Session();
		void addConnectioToSession();
	};
}
