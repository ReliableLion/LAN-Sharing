#pragma once
#include "stdafx.h"
#include "SocketException.h"
#include "TimeoutException.hpp"
#include <memory>
#include <iomanip>
#include <ctime>

namespace session {

	class TCPConnection {
	private:
		SOCKET sock = 0;
		SOCKADDR_IN remote_address;
		const long sec = 10;
		const long usec = 0;
	private:
		size_t readline_unbuffered(char *vptr, size_t maxlen);
	public:
		TCPConnection();
		bool acceptConnection(SOCKET s);
		bool closeConnection();
		void printEndpointInfo();
		bool recvall(char *data, int totalByte, int& totalReadByte);
		bool sendall(const char *data, int totalByte, int& totalSentByte);
		bool readline(char *data, int& readByte, int maxByte);
	};

	typedef std::shared_ptr<TCPConnection> conn_ptr;

}