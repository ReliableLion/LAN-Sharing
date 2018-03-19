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

	std::string get_client_address(struct sockaddr_in *client_address_ptr);

	class udp_client {

	public:

		udp_client::udp_client();
		bool get_adapter();

		void udp_client::get_server_info(std::string address, std::string port);
		int udp_client::send_datagram(std::string buff) const;
		int udp_client::receive_datagram();
		void udp_client::send_broadcast(const char* message);
		map<string, string> udp_client::get_online_users();

		udp_client::~udp_client();

	private:
		int sock;
		char buffer_[MAXBUFL] = "";
		char server_address_[INET_ADDRSTRLEN];
		char client_address_[INET_ADDRSTRLEN] = "";
		int server_port_;
		struct sockaddr_in broadcast_address_, server_address_struct_;

	};

	class udp_server {

	public:
		udp_server::udp_server();

		int udp_server::send_datagram(char *buffer, const struct sockaddr_in *saddr, socklen_t addr_len, size_t len) const;
		socklen_t udp_server::receive_datagram(char *buffer, const struct sockaddr_in *caddr, size_t len) const;

		udp_server::~udp_server();

	private:
		int server_sock_;
		char buffer_[MAXBUFL] = "";
		char server_address_[INET_ADDRSTRLEN];

	};



}
