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

	class udp_client {

	public:

		udp_client::udp_client();

		void udp_client::get_server_info(std::string address, std::string port);
		void udp_client::send_datagram(std::string buff) const;
		int udp_client::receive_datagram();
		void udp_client::send_broadcast();
		map<string, string> udp_client::get_online_users();

		udp_client::~udp_client();

	private:
		int sock;
		char buffer_[MAXBUFL] = "";
		char server_address_[INET_ADDRSTRLEN];
		int server_port_;
		struct sockaddr_in broadcastAddress, server_address;

	};

	class udp_server {

	public:
		udp_server::udp_server();

		void udp_server::send_datagram(char *buffer, const struct sockaddr_in *saddr, socklen_t addr_len, size_t len) const;
		socklen_t udp_server::receive_datagram(char *buffer, const struct sockaddr_in *caddr, size_t len) const;
		void udp_server::sendHello(const struct sockaddr_in *saddr, socklen_t addr_len, size_t len) const;

		udp_server::~udp_server();

	private:
		int sock;
		char buffer_[MAXBUFL] = "";
		char server_address_[INET_ADDRSTRLEN];

	};



}
