#pragma once
#include <WinSock2.h>
#include <string> //For std::string
#include "Constants.hpp"

namespace session {

	class TCPConnection {

	public:
		TCPConnection() {};
		bool connectTo(const char * ip, const int port);
		bool recvall(char * data, int totalBytes) {};
		bool recvToEndl(char * data) {};
		size_t readline_unbuffered(int fd, void *vptr, size_t maxlen) {};
		bool closeConnection() {};
		~TCPConnection() {};

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