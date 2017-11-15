#include "UDPService.hpp"
#include "Exceptions.hpp"

using namespace udp_service;

UDPClient::UDPClient(std::string address, std::string port) {

	struct addrinfo hints, *res, *res0;
	struct sockaddr_in* server_address;

	memset(&server_address, 0, sizeof(server_address));

	/*******************************************************************************************/

	/*
	* Risoluzione IPV4 UDP
	*/

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	if (getaddrinfo(address.c_str(), port.c_str(), &hints, &res0) != 0) {
		throw udp_exception::udpException("--- Error getaddrinfo!\n");
	}

	sock = -1;
	for (res = res0; res != NULL; res = res->ai_next) {

		sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if (socket < 0)
			throw udp_exception::udpException("--- Error socket() failed!\n");


		cout << "--- Socket created, fd number: " << socket << endl;

		break;
	}

	if (res != NULL)
		server_address = (struct sockaddr_in *)res->ai_addr;
	else 
		throw udp_exception::udpException("- Error couldn't connect! No server found!");

	inet_ntop(AF_INET, &(server_address->sin_addr), serverAddress, INET_ADDRSTRLEN);
	serverPort = ntohs(server_address->sin_port);

	freeaddrinfo(res0);

	/*******************************************************************************************/

}

void UDPClient::send_datagram(char *buffer, const struct sockaddr_in saddr) {

	/* send datagram */

	sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&saddr, sizeof(saddr));

	cout << "(%s) --- Data has been sent!\n" << endl;
}

int UDPClient::receive_datagram(char *buffer, const struct sockaddr_in saddr) {

	struct timeval tval;
	fd_set cset;
	socklen_t address_len = sizeof(saddr);
	size_t n;

	FD_ZERO(&cset);
	FD_SET(sock, &cset);
	tval.tv_sec = TIMEOUT; // 3 seconds timeout
	tval.tv_usec = 0;


	if ((n = select(sock + 1, &cset, NULL, NULL, &tval)) == -1)
		throw udp_exception::udpException("--- Error select() failed\n");

	if (n > 0) {

		/*
		* ****************
		* SOCKET SELECTED
		* ****************
		*/
		buffer[0] = 0;

		n = recvfrom(sock, buffer, MAXBUFL, 0, (struct sockaddr*)&saddr, &address_len);

		cout << "--- Received string " << buffer << endl;

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

		cout <<"No response after " << TIMEOUT << " seconds!" << endl; // tval is reset by select

		return 1;
	}
}


UDPServer::UDPServer() {

	struct sockaddr_in* server_address, client_address, *client_address_ptr;
	socklen_t address_len;

	client_address_ptr = &client_address;
	ZeroMemory(&client_address, sizeof(client_address));

	/* create socket */
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* specify address to bind to */
	memset(&server_address, 0, sizeof(server_address));
	server_address->sin_family = AF_INET;
	server_address->sin_port = htons((uint16_t)UDP_PORT);
	server_address->sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sock, (sockaddr*)&server_address, sizeof(server_address));

	cout << "--- Socket created!" << endl;


	while (1) {

		inet_ntop(AF_INET, &(server_address->sin_addr), serverAddress, INET_ADDRSTRLEN);

		cout << "--- Listening on " << serverAddress << ":" << ntohs(server_address->sin_port) << endl;

		address_len = receive_datagram(buffer, client_address_ptr, MAXBUFL);

		send_datagram(buffer, client_address_ptr, address_len, strlen(buffer));
	}

}

void UDPServer::send_datagram(char *buffer, const struct sockaddr_in *saddr, socklen_t addr_len, size_t len) {

	sendto(sock, buffer, len, 0, (struct sockaddr*)saddr, addr_len); // strlen(buffer) because I want to send just the valid characters.

	cout << "--- Data has been sent!" << endl;
}

socklen_t UDPServer::receive_datagram(char *buffer, const struct sockaddr_in *caddr, size_t len) {

	socklen_t address_len = sizeof(caddr);
	size_t n;

	n = recvfrom(sock, buffer, len, 0, (struct sockaddr*)caddr, &address_len);

	if (n > (len))
		cout << "--- Some bytes lost!" << endl;

	return address_len;
}
