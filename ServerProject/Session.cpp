#include "Session.hpp" 
#include "stdafx.h"

session::TCPConnection::TCPConnection() {}

bool session::TCPConnection::accept_connection(const SOCKET s) {
	int addrlen = sizeof(remote_address);
	sock = accept(s, (SOCKADDR*)&remote_address, &addrlen);

	if (sock ==  SOCKET_ERROR) {								// check if the scoket return an error
		std::cout << "server refused the connection, error: " << std::to_string(WSAGetLastError()) << std::endl;
		return false;
	}
	
	return true;
}

bool session::TCPConnection::close_connection() const {

	if (closesocket(sock) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK)
			//If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
			return true; //return true since connection has already been closed

		std::cout << "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + "!";
		return false;
	}

	return true;
}

void session::TCPConnection::print_endpoint_info() const {
	if (sock == 0) {
		std::cout << "socket not connected" << std::endl;
		return;
	}
	std::cout << "IP address: " << remote_address.sin_addr.s_addr << std::endl;
	std::cout << "port number: " << remote_address.sin_port << std::endl << std::endl;
}

/**
 * \brief 
 * \param data : buffer filled with data received
 * \param totalByte : number of bytes to be read
 * \param totalReadByte : number of Bytes really read
 * \return false if the connection has been closed by peer, otherside true
 */
bool session::TCPConnection::recvall(char *data, int totalBytes, int& totalReadBytes) {
	struct timeval time;
	int byteReceived = 0;
	FD_SET read_sock;

	FD_ZERO(&read_sock);
	FD_SET(sock, &read_sock);

	while (byteReceived < totalBytes) {
		time.tv_sec = sec; 
		time.tv_usec = usec;

		int result = select(1, &read_sock, nullptr, nullptr, &time);

		if (result == SOCKET_ERROR) {
			throw SocketException(WSAGetLastError());
		} 
		if (result == 0) {
			throw TimeoutException();
		}

		if (result == 1) {

			int byteRead = recv(sock, data + byteReceived, totalBytes - byteReceived, 0);
			if (byteRead == SOCKET_ERROR) {
				throw SocketException(WSAGetLastError());
			}
			if (byteRead == 0) {
				totalReadBytes = byteReceived;
				return false;
			}

			byteReceived += byteRead;					// increment the number of bytes received

		} else {
			totalReadBytes = byteReceived;
			return false;
		}
	}
	totalReadBytes = byteReceived;
	return true;
}

/**
 * \brief 
 * \param data : buffer used to send the data
 * \param totalByte : # of bytes to be sent
 * \param totalSentByte : reference type, total number of bytes really sent
 * \return false if the connection has been closed by peer, true if the data has been sent correctly
 */
bool session::TCPConnection::sendall(const char* data, const int totalBytes, int& totalSentBytes) {
	int uploadedBytes = 0;
	
	while (uploadedBytes < totalBytes) {
		int sentBytes = send(sock, data + uploadedBytes, totalBytes - uploadedBytes, 0);
		if (sentBytes == 0) {
			return false;
		} else 
		if (sentBytes == SOCKET_ERROR) {
			throw SocketException(WSAGetLastError());
		}
		uploadedBytes += sentBytes;
	}
	return true;
}

/**
 * \brief 
 * \param data : buffer used to write the dato 
 * \param readByte : reference value, total number of byte read
 * \param maxByte : max # of Bytes that can be read
 * \return false if the connection has been closed by peer, true if the data are read correclty
 */
bool session::TCPConnection::readline(char *data, const int maxBytes, int& readBytes) {
	int rByte = readline_unbuffered(data, (size_t)maxBytes);

	if (rByte == 0) {
		return false;
	}  
	if (rByte < 0) {
		throw SocketException(WSAGetLastError());
	}  
	if (rByte > 0) {
		readBytes = rByte;
		return true;
	}
}

//					PRIVATE METHODS

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