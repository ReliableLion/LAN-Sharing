#pragma once
#include <WinSock2.h>
#include <string> //For std::string
#include "Constants.hpp"
#include <Ws2tcpip.h> //for inet_pton
#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <iostream> //for std::cout
#include <memory>

namespace session {

	class TCPConnection {

	public:
		TCPConnection() {};
		bool TCPConnection::connect_to(const char * ip, const int port);
		bool TCPConnection::recvall(char * data, int totalBytes) const;
		bool TCPConnection::readline(char * data) const;
		size_t TCPConnection::readline_unbuffered(char *vptr, size_t maxlen) const;
		bool transmit_file_handler(std::string file_name, std::string file_path) const;
		bool TCPConnection::close_connection() const;
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

	typedef std::shared_ptr<TCPConnection> conn_ptr;

}
