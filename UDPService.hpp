#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>

#include "Constants.hpp"
#include <map>

// link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

namespace udp_service {

	class UDPClient {

	public:

		UDPClient::UDPClient();

		void UDPClient::get_server_info(std::string address, std::string port);
		void UDPClient::send_datagram(const struct sockaddr_in saddr) const;
		int UDPClient::receive_datagram(const struct sockaddr_in saddr);
		void UDPClient::send_broadcast();

		UDPClient::~UDPClient();

	private:
		int sock;
		char buffer[MAXBUFL] = "";
		char serverAddress[INET_ADDRSTRLEN];
		int serverPort;
		struct sockaddr_in broadcastAddress, server_address;

	};

	class UDPServer {

		UDPServer::UDPServer();

		void UDPServer::send_datagram(char *buffer, const struct sockaddr_in *saddr, socklen_t addr_len, size_t len) const;
		socklen_t UDPServer::receive_datagram(char *buffer, const struct sockaddr_in *caddr, size_t len) const;
		void UDPServer::sendHello(const struct sockaddr_in *saddr, socklen_t addr_len, size_t len) const;

		map<char*, string> get_online_users(const struct sockaddr_in saddr);

		UDPServer::~UDPServer();

	private:
		int sock;
		char buffer[MAXBUFL] = "";
		char serverAddress[INET_ADDRSTRLEN];

	};



}
