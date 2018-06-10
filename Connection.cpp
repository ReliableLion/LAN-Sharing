#include "Exceptions.hpp"
#include "Connection.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace connection;

TcpConnection::TcpConnection(const std::string host, const int port) : alive_(true) {
    int n;
    struct addrinfo hints, *res, *res0;

    if (port < 0 || port > 65535) throw SocketException(1);

	auto char_port = std::to_string(port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if ((n = getaddrinfo(host.c_str(), char_port.c_str(), &hints, &res0)) != 0)
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
            closesocket(sock_);    /* bind error, close and try next one */
            continue;
        }

        remote_address_ = *reinterpret_cast<struct sockaddr_in *>(res->ai_addr);
        break; /* Ok we got one */
    }

    freeaddrinfo(res0);

    if (res == nullptr) {    /* errno set from final connect() */
        std::cout << "Couldn't connect to " << host << ":" << port << " because of error " << WSAGetLastError()
                  << std::endl;
        throw SocketException(WSAGetLastError());
    }

	// allocate a receive buffer
	receive_buffer_ = new char[CHUNK];
}


TcpConnection::TcpConnection(const SOCKET socket, const SOCKADDR_IN socket_address) : alive_(true) {
    sock_ = socket;
    remote_address_ = socket_address;

//	int chunk = 65536;
//
//	setsockopt(sock_, SOL_SOCKET, SO_RCVBUF, (char*) &chunk, sizeof(chunk));

	receive_buffer_ = new char[CHUNK];
}

TcpConnection::~TcpConnection() {
	delete[] receive_buffer_;
}


/* 
 * close the conneciton 
 */
void TcpConnection::close_connection() const {

    if (alive_) {
        if (closesocket(sock_) == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAENOTSOCK)
                std::cout << "Failed to close the socket. Winsock Error: "
        					<<  std::to_string(WSAGetLastError()) << "!" << std::endl;
        }
    }

}

/*
 * print the information about the remote end point
 */
void TcpConnection::print_endpoint_info() const {
    char client_address[1024];
    inet_ntop(AF_INET, &(remote_address_.sin_addr), client_address, 1024);

    if (sock_ == 0) {
        std::cout << "socket not connected" << std::endl;
        return;
    }

    std::cout << "IP address: " << client_address << std::endl;
    std::cout << "port number: " << ntohs(remote_address_.sin_port) << std::endl << std::endl;
}

bool TcpConnection::read_data(std::shared_ptr<SocketBuffer> buffer) {

	auto bytes_read = 0;

	// clear the buffer before we read data
	buffer->clear();

	bytes_read = read_select(receive_buffer_, buffer->get_max_size());

	if (bytes_read == SOCKET_ERROR) throw SocketException(WSAGetLastError());

	// the connection is closed
	if (bytes_read == 0) alive_ = false;

	if (bytes_read == 1 && receive_buffer_[0] == '\0')
		std::cout << "the receive return an empty buffer" << std::endl;
	else 
		buffer->replace(receive_buffer_, bytes_read);

    return alive_;
}

bool TcpConnection::send_data(std::shared_ptr<SendSocketBuffer> buffer) const {
    const auto total_bytes = buffer->get_size();
	auto sent_bytes = 0;

	if (total_bytes == 0) {
		std::cout << "the buffer is empty " << std::endl;
		return true;
	}

    while (buffer->get_bytes_sent() < total_bytes) {
		auto temp = buffer->get_bytes_sent();
        sent_bytes = send(sock_, buffer->get_remaining_data(), buffer->get_remaining_bytes(), 0);

        if (sent_bytes == 0) return false;

        if (sent_bytes == SOCKET_ERROR) throw SocketException(WSAGetLastError());

        buffer->send(sent_bytes);
    }

    return true;
}


bool TcpConnection::read_line(std::shared_ptr<SocketBuffer> buffer) const {

    // TODO ricordarsi di controllare se il buffer finisce con \r\n
	auto read_byte = 0;
//    auto *local_buffer = new char[buffer->get_max_size()];
//    memset(local_buffer, 0, static_cast<std::size_t> (buffer->get_max_size()));

    struct timeval time;
    FD_SET read_sock;

    FD_ZERO(&read_sock);
    FD_SET(sock_, &read_sock);

    time.tv_sec = SEC_;
    time.tv_usec = USEC_;

    const auto result = select(sock_ + 1, &read_sock, nullptr, nullptr, &time);

    if (result == SOCKET_ERROR) throw SocketException(WSAGetLastError());

    if (result == 0) throw TimeoutException();

	// use the function readline to read until a \r\n is reached
    if (result > 0) read_byte = readline_unbuffered(receive_buffer_, buffer->get_max_size());

	// it means that the connection has been closed
    if (read_byte == 0) return false;

    if (read_byte < 0) throw SocketException(WSAGetLastError());

	// if an empty string is received than is not insert into the buffer
	if (read_byte == 1 && receive_buffer_[0] == '\0') {
		buffer->clear();
		std::cout << "received an empty buffer" << std::endl;
		return true;
	}

	// add the result of the readline 
    buffer->replace(receive_buffer_, read_byte);
    return true;
}

SOCKET TcpConnection::get_handle_socket() {
	return sock_;
}

size_t TcpConnection::readline_unbuffered(char *vptr, const int maxlen) const {

    size_t n, rc;
    char c;

	auto ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ((rc = recv(sock_, &c, 1, 0)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;    /* newline is stored, like fgets() */
        } else if (rc == 0) {
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break; /* EOF, some data was read */
        } else
            return -1; /* error, errno set by read() */
    }
    *ptr = 0; /* null terminate like fgets() */

    return n;
}

int TcpConnection::read_select(char *read_buffer, const int size) const {
    struct timeval time;
    FD_SET read_sock;

    FD_ZERO(&read_sock);
    FD_SET(sock_, &read_sock);

    time.tv_sec = SEC_;
    time.tv_usec = USEC_;

    const int result = select(sock_ + 1, &read_sock, nullptr, nullptr, &time);

    if (result == SOCKET_ERROR) throw SocketException(1);

    if (result == 0) throw TimeoutException();

	 //receive the data from the socket 
    if (result > 0) return recv(sock_, read_buffer, size, 0);

	return 0;
}
