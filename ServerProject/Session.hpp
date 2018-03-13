#pragma once
#include "stdafx.h"
#include "Listen_socket.h"
#include "SocketException.h"
#include "WinSock2.h"
#include "WS2tcpip.h"
#include "TimeoutException.hpp"
#include <memory>
#include <iomanip>
#include <ctime>

namespace session {

class TCPConnection {
private:
	SOCKET sock = 0;
	SOCKADDR_IN remote_address;
	const long sec = TIMEOUT_SEC;
	const long usec = TIMEOUT_USEC;
private:
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

	typedef std::shared_ptr<TCPConnection> conn_ptr;

}