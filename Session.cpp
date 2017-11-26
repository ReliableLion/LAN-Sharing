#include "Session.hpp"
#include "FileHandler.hpp"
#include "Exceptions.hpp"
#include <iostream> //for std::cout
#include "Constants.hpp"
#include <Ws2tcpip.h> //for inet_pton
using namespace session;

bool TCPConnection::connect_to(const char * ip, const int port) {

	char server_address[INET_ADDRSTRLEN];

	inet_pton(AF_INET, ip, &remote_address.sin_addr.s_addr); //Server Address [127.0.0.1] = localhost
	remote_address.sin_port = htons(port); //Port 
	remote_address.sin_family = AF_INET; //IPv4 Socket

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Set Connection socket
	const int sizeofaddr = sizeof(remote_address);
	inet_ntop(AF_INET, &(remote_address.sin_addr), server_address, INET_ADDRSTRLEN);

	if (connect(sock, reinterpret_cast<SOCKADDR*>(&remote_address), sizeofaddr) != 0) {
		std::cout << "ERROR CONNECT: " << std::to_string(WSAGetLastError()) << std::endl;

		std::cout << "Client failed to connect to: " << server_address << ":" << ntohs(remote_address.sin_port) << std::endl << std::endl;

		return false;
	}

	std::cout << "Client Connected to: " << server_address << ":" << ntohs(remote_address.sin_port) << std::endl << std::endl;

	return true;
}

bool TCPConnection::recvall(char * data, const int total_bytes) const {
	
	auto bytes_received = 0; //Holds the total bytes received
	while (bytes_received < total_bytes) { //While we still have more bytes to recv 

		const auto retn_check = recv(sock, data + bytes_received, total_bytes - bytes_received, 0); //Try to recv remaining bytes
		if (retn_check == SOCKET_ERROR || retn_check == 0) //If there is a socket error while trying to recv bytes or if connection is lost
			return false; //Return false - failed to recvall
		bytes_received += retn_check; //Add to total bytes received
	}
	return true; //Success!
}

bool TCPConnection::readline(char * data) const {
	
	const auto nread = readline_unbuffered(data, MAXBUFL);
	if (nread == 0) {
		return false;
	}
	if (nread == -1) {
		std::cout << "(%s) error - readline() failed" << std::endl;

		throw SOCKET_ERROR;
	}

	return true;
}

size_t TCPConnection::readline_unbuffered(char *vptr, const size_t maxlen) const {

	size_t n, rc;
	char c;

	auto ptr = vptr;
	for (n = 1; n<maxlen; n++)
	{
		if ((rc = recv(sock, &c, 1, 0)) == 1)
		{
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		}
		else if (rc == 0)
		{
			if (n == 1)
				return 0; /* EOF, no data read */
			break;
			/* EOF, some data was read */
		}
		else
			return -1; /* error, errno set by read() */
	}
	*ptr = 0; /* null terminate like fgets() */

	return n;
}

bool TCPConnection::transmit_file_handler(const std::string file_name, const std::string file_path) const {

	TransmitFileHandler handler(file_name, file_path);

	try {
		if (handler.openFile())
			return handler.transmit_file(sock);
	}
	catch (transmit_file_exception& error) {
		std::cout << error.what() << std::endl;
	}
	catch (file_open_exception& error) {
		std::cout << error.what() << std::endl;
	}

	return false;
}

/*
* Return values:
*
*  > 0   = # characters read
*
*  -1  = an error occured
*/
size_t TCPConnection::send_all(const char *ptr, const size_t nbytes) const {

	size_t nleft;

	for (nleft = nbytes; nleft>0;) {
		const size_t nwritten = send(sock, ptr, nleft, 0);
		if (nwritten == -1) /* Error */
			return(nwritten);
		else {
			nleft -= nwritten;
			ptr += nwritten;
		}
	}

	return (nbytes - nleft);
}

bool TCPConnection::close_connection() const {

	if (closesocket(sock) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
			return true; //return true since connection has already been closed

		std::cout << "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + "!";
		return false;
	}

	return true;
}

TCPConnection::~TCPConnection() {

	close_connection();
}