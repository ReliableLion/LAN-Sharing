#include "Session.hpp" 
#include "stdafx.h"

session::TCPConnection::TCPConnection() {}

bool session::TCPConnection::acceptConnection(SOCKET s) {
	int addrlen = sizeof(remote_address);
	sock = accept(s, (SOCKADDR*)&remote_address, &addrlen);
	if (sock ==  SOCKET_ERROR) {
		std::cout << "server refused the connection, error: " << std::to_string(WSAGetLastError()) << std::endl;
		return false;
	}
	else {
		return true;
	}
}

bool session::TCPConnection::closeConnection() {
	if (closesocket(sock) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
			return true; //return true since connection has already been closed

		std::cout << "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + "!";
		return false;
	}

	return true;
}

void session::TCPConnection::printEndpointInfo() {
	if (sock == 0) {
		std::cout << "socket not connected" << std::endl;
		return;
	}
	std::cout << "IP address: " << remote_address.sin_addr.s_addr << std::endl;
	std::cout << "port number: " << remote_address.sin_port << std::endl << std::endl;
}

bool session::TCPConnection::recvall(char *data, int totalByte, int& totalReadByte) {
	struct timeval time;
	int byteReceived = 0;
	FD_SET readSock;

	FD_ZERO(&readSock);
	FD_SET(sock, &readSock);

	while (byteReceived < totalByte) {
		time.tv_sec = sec; 
		time.tv_usec = usec;
		int result = select(1, &readSock, NULL, NULL, &time);

		if (result == SOCKET_ERROR) {
			throw SocketException(WSAGetLastError());
		} else 
		if (result == 0) {
			throw TimeoutException();
		} else 
		if (result == 1) {

			int byteRead = recv(sock, data + byteReceived, totalByte - byteReceived, 0);
			if (byteRead == SOCKET_ERROR) {
				throw SocketException(WSAGetLastError());
			} else
			if (byteRead == 0) {
				totalReadByte = byteReceived;
				return false;
			}
			else {
				byteReceived += byteRead;
			}

		} else {
			totalReadByte = byteReceived;
			return false;
		}
	}
	totalReadByte = byteReceived;
	return true;
}

bool session::TCPConnection::sendall(const char* data, int totalByte, int& totalSentByte) {
	int uploadedByte = 0;
	
	while (uploadedByte < totalByte) {
		int sentByte = send(sock, data + uploadedByte, totalByte - uploadedByte, 0);
		if (sentByte == 0) {
			return false;
		} else 
		if (sentByte == SOCKET_ERROR) {
			throw SocketException(WSAGetLastError());
		}
		uploadedByte += sentByte;
	}
	return true;
}

bool session::TCPConnection::readline(char *data, int& readByte, int maxByte) {
	int rByte = readline_unbuffered(data, (size_t)maxByte);

	if (rByte == 0) {
		return false;
	} else 
	if (rByte < 0) {
		throw SocketException(WSAGetLastError());
	} else 
	if (rByte > 0) {
		readByte = rByte;
		return true;
	}
}

size_t session::TCPConnection::readline_unbuffered(char *vptr, size_t maxlen) {

	size_t n, rc;
	char c, *ptr;

	ptr = vptr;
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
			else
				break; /* EOF, some data was read */
		}
		else
			return -1; /* error, errno set by read() */
	}
	*ptr = 0; /* null terminate like fgets() */

	return n;
}