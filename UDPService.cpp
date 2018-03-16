#include "UDPService.hpp"
#include "Exceptions.hpp"
#include "Message.hpp"
#include <chrono>

using namespace udp_service;

udp_client::udp_client(): sock(0), server_port_(0) {
	//Set broadcast address
	broadcast_address_.sin_family = AF_INET;
	broadcast_address_.sin_addr.s_addr = htonl(0xffffffff); //broadcast address
	broadcast_address_.sin_port = htons(UDP_PORT);
}

void udp_client::get_server_info(std::string address, std::string port) {

	struct addrinfo hints, *res, *res0;

	memset(&server_address_struct_, 0, sizeof(server_address_struct_));

	/*******************************************************************************************/

	/*
	* Risoluzione IPV4 UDP
	*/

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	if (getaddrinfo(address.c_str(), port.c_str(), &hints, &res0) != 0) {
		throw udp_exception::udp_exception("--- Error getaddrinfo!\n");
	}

	sock = -1;
	for (res = res0; res != nullptr; res = res->ai_next) {

		sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if (socket < nullptr) {
			cout << "--- Error socket() failed!\n" << endl;
			continue;
		}

		cout << "--- Socket created, fd number: " << socket << endl;
		break;

	}

	if (res != nullptr) {
		server_address_struct_ = *(reinterpret_cast<const sockaddr_in*>(res->ai_addr));
	}
	else {
		cout << "NULL: " << WSAGetLastError() << endl;

		throw udp_exception::udp_exception("- Error couldn't connect! No server found!");
	}

	inet_ntop(AF_INET, &(server_address_struct_.sin_addr), server_address_, INET_ADDRSTRLEN);
	server_port_ = ntohs(server_address_struct_.sin_port);

	freeaddrinfo(res0);

	/*******************************************************************************************/

}

void udp_client::send_datagram(std::string buff) const {

	/* send datagram */

	sendto(sock, buff.c_str(), buff.size(), 0, reinterpret_cast<const struct sockaddr*>(&server_address_struct_), sizeof(server_address_struct_));

	cout << "(%s) --- Data has been sent!\n" << endl;
}

int udp_client::receive_datagram() {

	struct timeval tval;
	fd_set cset;
	socklen_t address_len = sizeof(server_address_struct_);
	size_t n;

	FD_ZERO(&cset);
	FD_SET(sock, &cset);
	tval.tv_sec = TIMEOUT;
	tval.tv_usec = 0;

	if ((n = select(sock + 1, &cset, nullptr, nullptr, &tval)) == -1)
		throw udp_exception::udp_exception("--- Error select() failed\n");

	if (n > 0) {

		/*
		* ****************
		* SOCKET SELECTED
		* ****************
		*/
		buffer_[0] = 0;

		n = recvfrom(sock, buffer_, MAXBUFL, 0, const_cast<struct sockaddr*>(reinterpret_cast<const struct sockaddr*>(&server_address_struct_)), &address_len);

		cout << "--- Received string " << buffer_ << endl;

		if (n > (MAXBUFL))
			cout << "--- Some bytes lost!" << endl;

		return 0;

	}
	else {

		/*
		* ********
		* TIMEOUT
		* ********
		*/

		cout << "No response after " << TIMEOUT << " seconds!" << endl; // tval is reset by select

		return 1;
	}
}

void udp_client::send_broadcast() {
	
	auto broadcast = 1;

	// Create a UDP socket
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		throw udp_exception::udp_exception("--- Error socket() failed!\n");

	// Set socket options to broadcast
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char *>(&broadcast), sizeof(broadcast)) < 0)
		if (closesocket(sock) != 0)
			throw udp_exception::udp_exception("--- Error closesocket() failed!\n");

	// Broadcast data on the socket
	if (sendto(sock, DISCOVERY_MSG, strlen(DISCOVERY_MSG) + 1, 0, reinterpret_cast<sockaddr *> (&broadcast_address_), sizeof(broadcast_address_)) < 0)
		if (closesocket(sock) != 0)
			throw udp_exception::udp_exception("--- Error closesocket() failed!\n");

	cout << "(%s) --- Broadcast packet has been sent!\n" << endl;

	if (closesocket(sock) != 0)
		throw udp_exception::udp_exception("--- Error closesocket() failed!\n");

}

//Todo Move this method to Discovery.cpp
map<string, string> udp_client::get_online_users() {

	map<string, string> online_users;
	char client_address[INET_ADDRSTRLEN];

	struct timeval tval;
	fd_set cset;
	socklen_t address_len = sizeof(server_address_struct_);
	size_t n;
	char username[USERNAME_LENGTH];
	discovery_message packet;

	using namespace std::chrono;
	const auto start = steady_clock::now();

	do {
		FD_ZERO(&cset);
		FD_SET(sock, &cset);
		tval.tv_sec = TIMEOUT;
		tval.tv_usec = 0;

		if ((n = select(sock + 1, &cset, nullptr, nullptr, &tval)) == -1)
			throw udp_exception::udp_exception("--- Error select() failed\n");

		if (n > 0) {

			/*
			* ****************
			* SOCKET SELECTED
			* ****************
			*/
			buffer_[0] = 0;

			n = recvfrom(sock, buffer_, MAXBUFL, 0, const_cast<struct sockaddr*>(reinterpret_cast<const struct sockaddr*>(&server_address_struct_)), &address_len);

			packet.Append(buffer_, strlen(buffer_));

			if (!packet.get_packet_type().compare(nullptr) && packet.get_packet_type() == HELLO_MSG) {

				cout << "--- Received string " << buffer_ << endl;

				packet.getUsername(username);

				inet_ntop(AF_INET, &(server_address_struct_.sin_addr), client_address, INET_ADDRSTRLEN);

				online_users.insert(make_pair(string(username), client_address));
			}

			if (n > (MAXBUFL))
				cout << "--- Some bytes lost!" << endl;
		}
		else {

			if (n == 0) {

				/*
				* ********
				* TIMEOUT
				* ********
				*/

				return online_users;
			}
			if (n == SOCKET_ERROR) {
				throw socket_exception(WSAGetLastError());
			}
		}
	} while (duration_cast<chrono::seconds>(steady_clock::now() - start) < 3s);

	return online_users;
}

udp_server::udp_server() {

	struct sockaddr_in server_address, client_address;
	discovery_message packet;

	const auto client_address_ptr = &client_address;
	ZeroMemory(&client_address, sizeof(client_address));

	/* create socket */
	server_sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* specify address to bind to */
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(uint16_t(UDP_PORT));
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(server_sock_, (sockaddr*)(&server_address), sizeof(server_address));

	inet_ntop(AF_INET, &(server_address.sin_addr), server_address_, INET_ADDRSTRLEN);

	cout << "--- Listening on " << server_address_ << ":" << ntohs(server_address.sin_port) << endl;

	/*Todo Move this block to Discovery.cpp
	while (true) {

		const auto address_len = receive_datagram(buffer_, client_address_ptr, MAXBUFL);

		packet.Append(buffer_, strlen(buffer_));

		if(!packet.get_packet_type().compare(nullptr) && packet.get_packet_type() == HELLO_MSG)
			send_datagram(buffer_, client_address_ptr, address_len, strlen(buffer_));
	}
	*/
}

void udp_server::send_datagram(char *buffer, const struct sockaddr_in *saddr, const socklen_t addr_len, const size_t len) const {

	char server_addr_[INET_ADDRSTRLEN];

	sendto(server_sock_, buffer, len, 0, reinterpret_cast<const struct sockaddr*>(saddr), addr_len); // strlen(buffer) because I want to send just the valid characters.

	inet_ntop(AF_INET, &(saddr->sin_addr), server_addr_, INET_ADDRSTRLEN);


	cout << "--- Server has responded at: " << server_addr_ << endl;
}

void udp_server::send_hello(const struct sockaddr_in *saddr, const socklen_t addr_len, size_t len) const {

	sendto(server_sock_, HELLO_MSG, strlen(HELLO_MSG), 0, reinterpret_cast<const struct sockaddr*>(saddr), addr_len); // strlen(buffer) because I want to send just the valid characters.

	cout << "--- Data has been sent!" << endl;

}

socklen_t udp_server::receive_datagram(char *buffer, const struct sockaddr_in *caddr, const size_t length) const {

	socklen_t address_len = sizeof(struct sockaddr_in);

	const size_t n = recvfrom(server_sock_, buffer, length, 0, (struct sockaddr*)caddr, &address_len);

	char addr_string_[INET_ADDRSTRLEN];


	inet_ntop(AF_INET, &(caddr->sin_addr), addr_string_, INET_ADDRSTRLEN);

	cout << "Server received a message from: " << addr_string_ << endl;

	if (n > (length))
		cout << "--- Some bytes lost!" << endl;

	return address_len;
}

udp_client::~udp_client() {
	closesocket(sock);
}

udp_server::~udp_server() {
	closesocket(server_sock_);
}