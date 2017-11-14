#include "Session.hpp"

using namespace session;

bool TCPConnection::connectTo(const char * ip, const int port) {

	inet_pton(AF_INET, ip, &remote_address.sin_addr.s_addr); //Server Address [127.0.0.1] = localhost
	remote_address.sin_port = htons(port); //Port 
	remote_address.sin_family = AF_INET; //IPv4 Socket

	sock = socket(AF_INET, SOCK_STREAM, 0); //Set Connection socket
	int sizeofaddr = sizeof(remote_address);
	if (connect(sock, (SOCKADDR*)&remote_address, sizeofaddr) != 0) //If we are unable to connect...
	{
		std::cout << "Client failed to connect to: " << remote_address.sin_addr.s_addr << ":" << remote_address.sin_port << std::endl << std::endl;

		return false;
	}

	std::cout << "Client Connected to: " << remote_address.sin_addr.s_addr << ":" << remote_address.sin_port << std::endl << std::endl;

	return true;

}

bool TCPConnection::recvall(char * data, int totalBytes) {

	int bytesReceived = 0; //Holds the total bytes received
	while (bytesReceived < totalBytes) { //While we still have more bytes to recv 

		int RetnCheck = recv(sock, data + bytesReceived, totalBytes - bytesReceived, 0); //Try to recv remaining bytes
		if (RetnCheck == SOCKET_ERROR || RetnCheck == 0) //If there is a socket error while trying to recv bytes or if connection is lost
			return false; //Return false - failed to recvall
		bytesReceived += RetnCheck; //Add to total bytes received
	}
	return true; //Success!
}

bool TCPConnection::recvToEndl(char * data) {

	size_t nread = readline_unbuffered(sock, data, Constants::MAXBUFL);
	if (nread == 0) {
		return false;
	}
	else if (nread < 0) {
		std::cout << "(%s) error - readline() failed" << std::endl;

		return false;
	}

	return true;
}

size_t TCPConnection::readline_unbuffered(int fd, char *vptr, size_t maxlen) {

	size_t n, rc;
	char c, *ptr;

	ptr = vptr;
	for (n = 1; n<maxlen; n++)
	{
		if ((rc = recv(fd, &c, 1, 0)) == 1)
		{
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		}
		else if (rc == 0)
		{
			if (n == 1)
				return 0; /* EOF, no data read */
			else
				break; /* EOF, some data was read */
		}
		else
			return -1; /* error, errno set by read() */
	}
	*ptr = 0; /* null terminate like fgets() */

	return n;
}

bool TCPConnection::closeConnection() {

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

	closeConnection();
}