#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>

#include "Constants.hpp"

// link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

namespace udp_service {

	class UDPClient {

	public:

		UDPClient::UDPClient();

		void UDPClient::getServerInfo(std::string address, std::string port);
		void UDPClient::sendDatagram(char *buffer, const struct sockaddr_in saddr);
		int UDPClient::receiveDatagram(char *buffer, const struct sockaddr_in saddr);
		void UDPClient::sendBroadcast(char *buffer);

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

		void UDPServer::send_datagram(char *buffer, const struct sockaddr_in *saddr, socklen_t addr_len, size_t len);
		socklen_t UDPServer::receive_datagram(char *buffer, const struct sockaddr_in *caddr, size_t len);
		void UDPServer::sendHello(const struct sockaddr_in *saddr, socklen_t addr_len, size_t len);

		UDPServer::~UDPServer();

	private:
		int sock;
		char buffer[MAXBUFL] = "";
		char serverAddress[INET_ADDRSTRLEN];

	};



}
