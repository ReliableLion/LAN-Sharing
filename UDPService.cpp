#include "UDPService.hpp"
#include "Exceptions.hpp"

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

/*
* SENDING DATAGRAM
*/
void UDPClient::send_datagram(char *buffer, const struct sockaddr_in saddr) {

	/* send datagram */

	sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&saddr, sizeof(saddr));

	cout << "(%s) --- Data has been sent!\n" << endl;
}

/*
* RECEIVING DATAGRAM
*/
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