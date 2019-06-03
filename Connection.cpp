#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

#include "Exceptions.hpp"
#include "Connection.hpp"

using namespace connection;

TcpConnection::TcpConnection(const std::string host, const int port) : alive_(true) {
    int n;
    struct addrinfo hints, *res, *res0;

    if (port < 0 || port > 65535) 
		// the error code should be changed
		throw SocketException(1);

	auto char_port = std::to_string(port);

	ZeroMemory(&hints, sizeof(hints));
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

	if (res == nullptr) {    /* errno set from final connect() */
		std::cout << "Couldn't connect to " << host << ":" << port << " because of error "
			<< WSAGetLastError() << std::endl;
		throw SocketException(WSAGetLastError());
	}

    freeaddrinfo(res0);

	// allocate a receive buffer
	receive_buffer_ = new char[CHUNK];
}

/*
 *	This constructor should be used only if we have the connected socket and the 
 */
TcpConnection::TcpConnection(const SOCKET connected_socket, const SOCKADDR_IN remote_address) : alive_(true) {
    sock_ = connected_socket;
    remote_address_ = remote_address;

	// allocate a new buffer
	receive_buffer_ = new char[CHUNK];
}

TcpConnection::~TcpConnection() {
	this->close_connection();
	delete[] receive_buffer_;
}

/* 
 * close the connection 
 */
void TcpConnection::close_connection() const {

    if (alive_) {
        if (closesocket(sock_) == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAENOTSOCK)
				std::cout << "connection already closed by client" << std::endl;
        }
	}
}

/*
 * Print the information about the remote end point
 */
void TcpConnection::print_endpoint_info() const {
	if (sock_ == 0) {
		std::cout << "socket not connected" << std::endl;
		return;
	}
    char client_address[1024];
    inet_ntop(AF_INET, &(remote_address_.sin_addr), client_address, 1024);

    std::cout << "IP address: " << client_address << std::endl;
    std::cout << "port number: " << ntohs(remote_address_.sin_port) << std::endl << std::endl;
}

bool TcpConnection::read_data(std::shared_ptr<SocketBuffer> buffer) {

	// pay attenction because read select can throw an exception
	int bytes_read = 0;
	bytes_read = read_select(receive_buffer_, buffer->get_max_size());

	if (bytes_read == SOCKET_ERROR)
		throw SocketException(WSAGetLastError());
	else if (bytes_read == 0) {
		alive_ = false;
		return false;
	} else if (bytes_read == 1 && receive_buffer_[0] == '\0') {
		std::cout << "the receive return an empty buffer" << std::endl;
		return false;
	} else {
		buffer->replace(receive_buffer_, bytes_read);
		return true;
	}
}

/*
 * This method return false if no data has been sent, otherwise return true
 */
bool TcpConnection::send_data(std::shared_ptr<SocketBuffer> buffer) {

	if (buffer->get_size() == 0) {
		std::cout << "the buffer is empty " << std::endl;
		return false;
	}

	int bytes_sent;

    while (buffer->get_remaining_bytes() != 0) {
        bytes_sent = send(sock_, buffer->read(), buffer->get_remaining_bytes(), 0);

		if (bytes_sent == 0) {
			alive_ = false;
			return false;
		} else if (bytes_sent == SOCKET_ERROR) 
			throw SocketException(WSAGetLastError());

		buffer->update_read_ptr(bytes_sent);
    }

    return true;
}

bool TcpConnection::read_line(std::shared_ptr<SocketBuffer> buffer) {

	//throw SocketException(15);

    // remember to check if the buffer ends with /r/n
	int read_bytes = 0;

	// auto *local_buffer = new char[buffer->get_max_size()];
	// memset(local_buffer, 0, static_cast<std::size_t> (buffer->get_max_size()));

    struct timeval time;
    FD_SET read_sock;

    FD_ZERO(&read_sock);
    FD_SET(sock_, &read_sock);

    time.tv_sec = SEC_;
    time.tv_usec = USEC_;

    const auto result = select(sock_ + 1, &read_sock, nullptr, nullptr, &time);

	if (result == SOCKET_ERROR) {
		// int i = WSAGetLastError();
		throw SocketException(WSAGetLastError());
	}
    else if (result == 0) 
		throw TimeoutException();
	else 
		read_bytes = readline_unbuffered(receive_buffer_, buffer->get_max_size());

	if (read_bytes == 0) {
		alive_ = false;
		return false;
	} else if (read_bytes == 1 && receive_buffer_[0] == '\0') {
		buffer->clear();
		std::cout << "received an empty buffer" << std::endl;
		return false;
	} else if (read_bytes < 0)
		throw SocketException(WSAGetLastError());

	// add the result of the readline 
    buffer->replace(receive_buffer_, read_bytes);
    return true;
}

SOCKET TcpConnection::get_handle_socket() {
	return sock_;
}

size_t TcpConnection::readline_unbuffered(char *vptr, const int maxlen) {

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

int TcpConnection::read_select(char *read_buffer, const int size) {
    struct timeval time;
    FD_SET read_sock;

    FD_ZERO(&read_sock);
    FD_SET(sock_, &read_sock);

    time.tv_sec = SEC_;
    time.tv_usec = USEC_;

    const int result = select(sock_ + 1, &read_sock, nullptr, nullptr, &time);

    if (result == SOCKET_ERROR) 
		throw SocketException(1);
    else if (result == 0) 
		throw TimeoutException();
	else
		return recv(sock_, read_buffer, size, 0);
}

bool TcpConnection::get_connection_status() { return alive_; }

bool TcpConnection::read_n_data(std::shared_ptr<SocketBuffer> buffer, int n) { return true; }
