#include "stdafx.h"

#include "Connection.hpp" 

using namespace connection;

TCPConnection::TCPConnection(const char* host, const int port) : alive(true)
{
	int n;
	struct addrinfo hints, *res, *res0;

	if (port < 0 || port > 65535) throw SocketException(1);

	std::string char_port = std::to_string(port);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if ((n = getaddrinfo(host, char_port.c_str(), &hints, &res0)) != 0)
		std::cout << "Error within getaddrinfo(): " << WSAGetLastError() << std::endl;

	sock_ = -1;
	for (res = res0; res != nullptr; res = res->ai_next) {

		sock_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sock_ == INVALID_SOCKET) {
			std::cout << "(%s) socket() failed" << std::endl;
			continue;
		}

		std::cout << "(%s) --- Socket created, fd number: " << sock_ << std::endl;

		if (connect(sock_, res->ai_addr, res->ai_addrlen) < 0) {
			closesocket(sock_);	/* bind error, close and try next one */
			continue;
		}

		remote_address_ = *((struct sockaddr_in *)(res->ai_addr));
		break; /* Ok we got one */
	}

	freeaddrinfo(res0);

	if (res == NULL) {	/* errno set from final connect() */
		std::cout << "Couldn't connect to " << host << ":" << port << " because of error " << WSAGetLastError() << std::endl;
		throw SocketException(1);
	}
}


TCPConnection::TCPConnection(SOCKET socket, SOCKADDR_IN socket_address) : alive(true)
{
	sock_ = socket;
	remote_address_ = socket_address;
}


void TCPConnection::close_connection() const
{

	if (alive) {
		if (closesocket(sock_) == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAENOTSOCK)
				std::cout << "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + "!" << std::endl;
		}
	}

}

/**
* \brief print on standard output the information about the remote endpoint assoicated with the TCP connection
*/
void TCPConnection::print_endpoint_info() const {
	char client_address[1024];
	inet_ntop(AF_INET, &(remote_address_.sin_addr), client_address, 1024);

	if (sock_ == 0) {
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
bool TCPConnection::read_data(std::shared_ptr<SocketBuffer> buffer, int size) {

	if (size > buffer->get_max_size()) throw SocketException(1);

	auto *local_buffer = new char[buffer->get_max_size()];
	memset(local_buffer, 0, buffer->get_max_size());
	int bytes_received = 0, bytes_read = 0;

	buffer->clear();

	try
	{
		while (bytes_received < size &&  alive) {
			bytes_read = read_select(local_buffer, buffer->get_max_size());

			if (bytes_read == SOCKET_ERROR) {
				delete[] local_buffer;
				throw SocketException(WSAGetLastError());
			}

			// the connection is closed 
			if (bytes_read == 0) alive = false;
			else
			{
				bytes_received += bytes_read;					// increment the number of bytes received
				buffer->add(local_buffer, bytes_read);
			}

		}
	}
	catch (SocketException &se)
	{
		delete[] local_buffer;
		throw;
	}
	catch (TimeoutException &te)
	{
		delete[] local_buffer;
		throw;
	}

	// deallocate the local buffer
	delete[] local_buffer;

	return alive;
}

/**
* \brief
* \param data : buffer used to send the data
* \param totalByte : # of bytes to be sent
* \param totalSentByte : reference type, total number of bytes really sent
* \return false if the connection has been closed by peer, true if the data has been sent correctly
*/
bool TCPConnection::send_data(std::shared_ptr<SendSocketBuffer> buffer) {
	const int total_bytes = buffer->get_size();
	int sent_bytes = 0;

	while (buffer->get_bytes_sent() < total_bytes) {

		sent_bytes = send(sock_, buffer->get_buffer(), buffer->get_remaining_bytes(), 0);

		if (sent_bytes == 0) return false;

		if (sent_bytes == SOCKET_ERROR) throw SocketException(WSAGetLastError());

		buffer->send(sent_bytes);
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
bool TCPConnection::read_line(std::shared_ptr<SocketBuffer> buffer) {

	auto *local_buffer = new char[buffer->get_max_size()];
	memset(local_buffer, 0, buffer->get_max_size());

	const int read_byte = readline_unbuffered(local_buffer, buffer->get_max_size());

	if (read_byte == 0) return false;

	if (read_byte < 0) throw SocketException(WSAGetLastError());

	//	TODO	sistemare la parte di gestione dell'overflow della linea da leggere
	/*if (local_buffer[read_byte - 1] != '\n')
	{
		local_buffer[read_byte - 1] = ''
	}*/

	buffer->add(local_buffer, read_byte);
	return true;
}

size_t TCPConnection::readline_unbuffered(char *vptr, int maxlen) {

	size_t n, rc;
	char c, *ptr;

	ptr = vptr;
	for (n = 1; n<maxlen; n++) {
		if ((rc = recv(sock_, &c, 1, 0)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		}
		else if (rc == 0) {
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

size_t TCPConnection::read_select(char *read_buffer, int size)
{
	struct timeval time;
	FD_SET read_sock;

	FD_ZERO(&read_sock);
	FD_SET(sock_, &read_sock);

	time.tv_sec = sec_;
	time.tv_usec = usec_;

	const int result = select(sock_ + 1, &read_sock, nullptr, nullptr, &time);

	if (result == SOCKET_ERROR) throw SocketException(1);

	if (result == 0) throw TimeoutException();

	if (result > 0) return recv(sock_, read_buffer, size, 0);

}
