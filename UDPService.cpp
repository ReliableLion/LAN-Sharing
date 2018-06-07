#include "UDPService.hpp"
#include "Exceptions.hpp"
#include "Message.hpp"
#include <chrono>
#include <iphlpapi.h>
#include <iostream>
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

using namespace udp_service;

std::string udp_service::get_client_address(struct sockaddr_in *client_address_ptr) {

	char addr_string[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(client_address_ptr->sin_addr), addr_string, INET_ADDRSTRLEN);

	return addr_string;
}

UdpClient::UdpClient(): sock_(0), server_port_(0) {

	struct sockaddr_in source_address;

	//Set broadcast address
	broadcast_address_.sin_family = AF_INET;
	broadcast_address_.sin_addr.s_addr = htonl(0xffffffff); //broadcast address
	broadcast_address_.sin_port = htons(UDP_PORT);

	if (!get_adapter())
		throw udp_exception::UdpException("Cannot start UDP Client!");

	/* specify address to bind to */
	memset(&source_address, 0, sizeof(source_address));
	source_address.sin_family = AF_INET;
	source_address.sin_port = htons(uint16_t(UDP_PORT));
	inet_pton(AF_INET, client_address_, &(source_address.sin_addr));

	bind(sock_, reinterpret_cast<sockaddr*>(&source_address), sizeof(source_address));
}

bool UdpClient::get_adapter()
{
	// It is possible for an adapter to have multiple
	// IPv4 addresses, gateways, and secondary WINS servers
	// assigned to the adapter. 
	//
	// Note that this sample code only prints out the 
	// first entry for the IP address/mask, and gateway, and
	// the primary and secondary WINS server for each adapter. 

	PIP_ADAPTER_INFO p_adapter_info;
	PIP_ADAPTER_INFO p_adapter = nullptr;
	DWORD dw_ret_val = 0;

	ULONG ul_out_buf_len = sizeof(IP_ADAPTER_INFO);
	p_adapter_info = static_cast<IP_ADAPTER_INFO *>(MALLOC(sizeof(IP_ADAPTER_INFO)));
	if (p_adapter_info == nullptr) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return false;
	}

	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(p_adapter_info, &ul_out_buf_len) == ERROR_BUFFER_OVERFLOW) {
		FREE(p_adapter_info);
		p_adapter_info = static_cast<IP_ADAPTER_INFO *>(MALLOC(ul_out_buf_len));
		if (p_adapter_info == nullptr) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return false;
		}
	}

	if ((dw_ret_val = GetAdaptersInfo(p_adapter_info, &ul_out_buf_len)) == NO_ERROR) {
		p_adapter = p_adapter_info;

		while (p_adapter) {
			//printf("\tAdapter Name: \t%s\n", pAdapter->AdapterName);

			//printf("\tIP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);

			if (std::strcmp(p_adapter->AdapterName, NETWORK_ADAPTER_NAME) == 0) {
				strcpy_s(client_address_, sizeof(p_adapter->IpAddressList.IpAddress.String), p_adapter->IpAddressList.IpAddress.String);
			}

			p_adapter = p_adapter->Next;
		}
	}

	if (p_adapter_info)
		FREE(p_adapter_info);

	return true;
}

void UdpClient::get_server_info(std::string address, std::string port) {

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
		throw udp_exception::UdpException("Getaddrinfo error: "+std::to_string(WSAGetLastError()) + "\n");
	}

	sock_ = -1;
	for (res = res0; res != nullptr; res = res->ai_next) {

		sock_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if (sock_ == INVALID_SOCKET) {
			cout << "Socket() failed! Error: " << WSAGetLastError() << endl;
			continue;
		}
		break;
	}

	if (res != nullptr)
		server_address_struct_ = *(reinterpret_cast<const sockaddr_in*>(res->ai_addr));
	else
		throw udp_exception::UdpException("- Error couldn't connect! No server found!");

	inet_ntop(AF_INET, &(server_address_struct_.sin_addr), server_address_, INET_ADDRSTRLEN);
	server_port_ = ntohs(server_address_struct_.sin_port);

	freeaddrinfo(res0);

	/*******************************************************************************************/
}

int UdpClient::send_datagram(std::string buff) const {

	int n;

	/* send datagram */
	if ((n = sendto(sock_, buff.c_str(), buff.size(), 0, reinterpret_cast<const struct sockaddr*>(&server_address_struct_), sizeof(server_address_struct_)) == SOCKET_ERROR)) {
		cout << "Sendto failed! Error: " << WSAGetLastError() << endl;
		throw udp_exception::UdpException("Sendto error: " + std::to_string(WSAGetLastError()) + "\n");
	}

	return n;
}

int UdpClient::receive_datagram() {

	struct timeval tval;
	fd_set cset;
	socklen_t address_len = sizeof(server_address_struct_);
	size_t n;

	FD_ZERO(&cset);
	FD_SET(sock_, &cset);
	tval.tv_sec = TIMEOUT;
	tval.tv_usec = 0;

	if ((n = select(sock_ + 1, &cset, nullptr, nullptr, &tval)) == -1)
		throw udp_exception::UdpException("Select error: " + std::to_string(WSAGetLastError()) + "\n");

	if (n > 0) {

		/*
		* ****************
		* SOCKET SELECTED
		* ****************
		*/
		buffer_[0] = 0;

		n = recvfrom(sock_, buffer_, MAXBUFL, 0, const_cast<struct sockaddr*>(reinterpret_cast<const struct sockaddr*>(&server_address_struct_)), &address_len);

		if (n > (MAXBUFL))
			cout << "--- Some bytes lost!" << endl;

		return n;
	}
	/*
		* ********
		* TIMEOUT
		* ********
		*/

	cout << "No response after " << TIMEOUT << " seconds!" << endl; // tval is reset by select

	return 1;
}

void UdpClient::send_broadcast(const char* message) {
	
	auto broadcast = 1;
	struct sockaddr_in source_address;

	// Create a UDP socket
	if ((sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		throw udp_exception::UdpException("Socket error: " + std::to_string(WSAGetLastError()) + "\n");

	/* specify address to bind to */
	memset(&source_address, 0, sizeof(source_address));
	source_address.sin_family = AF_INET;
	source_address.sin_port = htons(uint16_t(UDP_PORT));
	inet_pton(AF_INET, client_address_, &(source_address.sin_addr));

	bind(sock_, reinterpret_cast<sockaddr*>(&source_address), sizeof(source_address));

	// Set socket options to broadcast
	if (setsockopt(sock_, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char *>(&broadcast), sizeof(broadcast)) < 0)
		if (closesocket(sock_) != 0)
			throw udp_exception::UdpException("Closesocket error: " + std::to_string(WSAGetLastError()) + "\n");

	// Broadcast data on the socket
	if (sendto(sock_, message, strlen(message) + 1, 0, reinterpret_cast<sockaddr *> (&broadcast_address_), sizeof(broadcast_address_)) < 0)
		if (closesocket(sock_) != 0)
			throw udp_exception::UdpException("Closesocket error: " + std::to_string(WSAGetLastError()) + "\n");

	cout << "Broadcast packet has been sent!\n" << endl;

	if (closesocket(sock_) != 0)
		throw udp_exception::UdpException("Closesocket error: " + std::to_string(WSAGetLastError()) + "\n");
}

//Todo Move this method to Discovery.cpp
map<string, string> UdpClient::get_online_users() {

	map<string, string> online_users;
	char client_address[INET_ADDRSTRLEN];

	struct timeval tval;
	fd_set cset;
	socklen_t address_len = sizeof(server_address_struct_);
	size_t n;
	char username[USERNAME_LENGTH];
	DiscoveryMessage packet;

	using namespace std::chrono;
	const auto start = steady_clock::now();

	do {
		FD_ZERO(&cset);
		FD_SET(sock_, &cset);
		tval.tv_sec = TIMEOUT;
		tval.tv_usec = 0;

		if ((n = select(sock_ + 1, &cset, nullptr, nullptr, &tval)) == -1)
			throw udp_exception::UdpException("--- Error select() failed\n");

		if (n > 0) {

			/*
			* ****************
			* SOCKET SELECTED
			* ****************
			*/
			buffer_[0] = 0;

			n = recvfrom(sock_, buffer_, MAXBUFL, 0, const_cast<struct sockaddr*>(reinterpret_cast<const struct sockaddr*>(&server_address_struct_)), &address_len);

            packet.append(buffer_, strlen(buffer_));

			if (packet.get_packet_type() == HELLO_MSG) {

				cout << "--- Received string " << buffer_ << endl;

				packet.get_username();

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
				throw SocketException(WSAGetLastError());
			}
		}
	} while (duration_cast<chrono::seconds>(steady_clock::now() - start) < 3s);

	return online_users;
}

UdpServer::UdpServer() {

	struct sockaddr_in server_address, client_address;

	const auto client_address_ptr = &client_address;
	ZeroMemory(&client_address, sizeof(client_address));

	/* create socket */
	if((server_sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
		cout << "Error while creating the socket " << WSAGetLastError() << endl;
		throw udp_exception::UdpException("Error while creating the socket " + std::to_string(WSAGetLastError()) + "\n");
	}

	/* specify address to bind to */
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(uint16_t(UDP_PORT));
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(server_sock_, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));

	inet_ntop(AF_INET, &(server_address.sin_addr), server_address_, INET_ADDRSTRLEN);

	cout << "--- Listening on " << server_address_ << ":" << ntohs(server_address.sin_port) << endl;
}

int UdpServer::send_datagram(const char *buffer, const struct sockaddr_in *saddr, const socklen_t addr_len, const size_t len) const {

	int n;

	if ((n = sendto(server_sock_, buffer, len, 0, reinterpret_cast<const struct sockaddr*>(saddr), addr_len)) == SOCKET_ERROR) { // strlen(buffer) because I want to send just the valid characters.
		cout << "Sendto failed! Error: " << WSAGetLastError() << endl;
		throw udp_exception::UdpException("Sendto error: " + std::to_string(WSAGetLastError()) + "\n");
	}

	return n;
}

socklen_t UdpServer::receive_datagram(char *buffer, const struct sockaddr_in *caddr, const size_t length) const {

	socklen_t address_len = sizeof(*caddr);

	const size_t n = recvfrom(server_sock_, buffer, length, 0, const_cast<struct sockaddr*>(reinterpret_cast<const struct sockaddr*>(caddr)), &address_len);

	if (n > (length))
		cout << "--- Some bytes lost!" << endl;
	else
		buffer[n] = '\0';

	return address_len;
}

UdpClient::~UdpClient() {
	closesocket(sock_);
}

UdpServer::~UdpServer() {
	closesocket(server_sock_);
}