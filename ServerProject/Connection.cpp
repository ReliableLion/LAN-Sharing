#include "Connection.hpp" 

using namespace connection;

TCPConnection::TCPConnection() {}

/**
 * \brief acppt an incoming TCP connection 
 * \param s receive the local socket used by the server 
 * \return TRUE if the connection is established, FALSE otherwise
 */
bool TCPConnection::accept_connection(Listen_socket s) {
	int addrlen = sizeof(remote_address);
	int sock_desc = s.getSocket();

	if(sock_desc == SOCKET_ERROR)
	{
		std::cout << "impossible to accept a connection, the local socket is empty" << std::endl;
		return false;
	}

	sock = accept(sock_desc, (SOCKADDR*)&remote_address, &addrlen);

	// check if the scoket return an error
	if (sock ==  SOCKET_ERROR) {								
		std::cout << "server refused the connection, error: " << std::to_string(WSAGetLastError()) << std::endl;
		return false;
	}
	
	return true;
}

bool TCPConnection::close_connection() const {

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

/**
 * \brief print on standard output the information about the remote endpoint assoicated with the TCP connection	
 */
void TCPConnection::print_endpoint_info() const {
	char client_address[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(remote_address.sin_addr), client_address, INET_ADDRSTRLEN);

	if (sock == 0) {
		std::cout << "socket not connected" << std::endl;
		return;
	}

	std::cout << "IP address: " << client_address << std::endl;
	std::cout << "port number: " << ntohs(remote_address.sin_port) << std::endl << std::endl;
}

/**
 * \brief 
 * \param data : buffer filled with data received
 * \param totalByte : number of bytes to be read
 * \param totalReadByte : number of Bytes really read
 * \return false if the connection has been closed by peer, otherside true
 */
bool TCPConnection::recvall(char *data, int totalBytes, int& totalReadBytes) {
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
bool TCPConnection::sendall(const char* data, const int totalBytes, int& totalSentBytes) {
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
bool TCPConnection::readline(char *data, const int maxBytes, int& readBytes) {
	int rByte = readline_unbuffered(data, (size_t) maxBytes);

	if (rByte == 0) {
		return false;
	}  else
	if (rByte < 0) {
		throw SocketException(WSAGetLastError());
	}  else
	if (rByte > 0) {
		readBytes = rByte;
		return true;
	}
}

//					PRIVATE METHODS

size_t TCPConnection::readline_unbuffered(char *vptr, size_t maxlen) {

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