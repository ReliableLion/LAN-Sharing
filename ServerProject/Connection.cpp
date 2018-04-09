#include "stdafx.h"

#include "Connection.hpp" 


using namespace connection;

TCPConnection::TCPConnection() {}

/**
 * \brief acppt an incoming TCP connection 
 * \param s receive the local socket used by the server 
 * \return TRUE if the connection is established, FALSE otherwise
 */
bool TCPConnection::accept_connection(Listen_socket s) {
	int addrlen = sizeof(remote_address_);
	int sock_desc = s.get_socket();

	if(sock_desc == SOCKET_ERROR)
	{
		std::cout << "impossible to accept a connection, the local socket is empty" << std::endl;
		return false;
	}

	socket_ = accept(sock_desc, (SOCKADDR*)&remote_address_, &addrlen);

	// check if the scoket return an error
	if (socket_ ==  SOCKET_ERROR) {								
		std::cout << "server refused the connection, error: " << std::to_string(WSAGetLastError()) << std::endl;
		return false;
	}
	
	return true;
}

bool TCPConnection::close_connection() const {

	if (closesocket(socket_) == SOCKET_ERROR)
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
	inet_ntop(AF_INET, &(remote_address_.sin_addr), client_address, INET_ADDRSTRLEN);

	if (socket_ == 0) {
		std::cout << "socket not connected" << std::endl;
		return;
	}

	std::cout << "IP address: " << client_address << std::endl;
	std::cout << "port number: " << ntohs(remote_address_.sin_port) << std::endl << std::endl;
}

/**
 * \brief 
 * \param data : buffer filled with data received
 * \param totalByte : number of bytes to be read
 * \param totalReadByte : number of Bytes really read
 * \return false if the connection has been closed by peer, otherside true
 */
bool TCPConnection::recv_all(char *data, int total_bytes, int& total_read_bytes) {
	struct timeval time;
	int byte_received = 0;
	FD_SET read_sock;

	FD_ZERO(&read_sock);
	FD_SET(socket_, &read_sock);

	while (byte_received < total_bytes) {
		time.tv_sec = sec_; 
		time.tv_usec = usec_;

		int select_result = select(1, &read_sock, nullptr, nullptr, &time);

		if (select_result == SOCKET_ERROR) {
			throw SocketException(WSAGetLastError());
		} 
		if (select_result == 0) {
			throw TimeoutException();
		}

		if (select_result == 1) {

			int byte_read = recv(socket_, data + byte_received, total_bytes - byte_received, 0);
			if (byte_read == SOCKET_ERROR) {
				throw SocketException(WSAGetLastError());
			}
			if (byte_read == 0) {
				total_read_bytes = byte_received;
				return false;
			}

			// increment the number of bytes received
			byte_received += byte_read;					

		} else {
			total_read_bytes = byte_received;
			return false;
		}
	}
	total_read_bytes = byte_received;
	return true;
}

/**
 * \brief 
 * \param data : buffer used to send the data
 * \param totalByte : # of bytes to be sent
 * \param totalSentByte : reference type, total number of bytes really sent
 * \return false if the connection has been closed by peer, true if the data has been sent correctly
 */
bool TCPConnection::send_all(const char* data, const int total_bytes, int& total_sent_bytes) {
	int uploaded_bytes = 0;
	
	while (uploaded_bytes < total_bytes) {
		int sent_bytes = send(socket_, data + uploaded_bytes, total_bytes - uploaded_bytes, 0);
		if (sent_bytes == 0) {
			return false;
		} else 
		if (sent_bytes == SOCKET_ERROR) {
			throw SocketException(WSAGetLastError());
		}
		uploaded_bytes += sent_bytes;
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
bool TCPConnection::read_line(char *data, const int max_bytes, int& read_bytes) {
	int read_byte = readline_unbuffered(data, (size_t) max_bytes);

	if (read_byte == 0) {
		return false;
	}  else
	if (read_byte < 0) {
		throw SocketException(WSAGetLastError());
	}  else
	if (read_byte > 0) {
		read_bytes = read_byte;
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
		if ((rc = recv(socket_, &c, 1, 0)) == 1)
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