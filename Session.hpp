#pragma once
#include <WinSock2.h>
#include <string> //For std::string
#include "Constants.hpp"
#include <Ws2tcpip.h> //for inet_pton
#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <iostream> //for std::cout

namespace session {

	class TCPConnection {

	public:
		TCPConnection() {};
		bool TCPConnection::connectTo(const char * ip, const int port);
		bool TCPConnection::recvall(char * data, int totalBytes);
		bool TCPConnection::recvToEndl(char * data);
		size_t TCPConnection::readline_unbuffered(int fd, char *vptr, size_t maxlen);
		bool TCPConnection::closeConnection();
		~TCPConnection();

	private:
		bool connection_state;
		SOCKET sock;
		SOCKADDR_IN remote_address;

	};

	class ProtocolManager {

	};

	class SessionManager {

		TCPConnection connection;

	};

}