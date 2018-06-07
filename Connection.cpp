#include "stdafx.h"

#include "Exceptions.hpp"
#include "Connection.hpp"
#include <iostream>
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
        throw SocketException(1);
    }
}


TcpConnection::TcpConnection(const SOCKET socket, const SOCKADDR_IN socket_address) : alive_(true) {
    sock_ = socket;
    remote_address_ = socket_address;
}


void TcpConnection::close_connection() const {

    if (alive_) {
        if (closesocket(sock_) == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAENOTSOCK)
                std::cout << "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + "!"
                          << std::endl;
        }
    }

}

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

bool TcpConnection::read_data(std::shared_ptr<SocketBuffer> buffer, const int size) {

    if (size > buffer->get_max_size()) throw SocketException(1);

    auto *local_buffer = new char[buffer->get_max_size()];
    memset(local_buffer, 0, static_cast<size_t>(buffer->get_max_size()));
	auto bytes_received = 0, bytes_read = 0;

    buffer->clear();

    try {
        while (bytes_received < size && alive_) {
            bytes_read = read_select(local_buffer, buffer->get_max_size());

            if (bytes_read == SOCKET_ERROR) {
                delete[] local_buffer;
                throw SocketException(WSAGetLastError());
            }

            // the connection is closed
            if (bytes_read == 0) alive_ = false;
            else {
                bytes_received += bytes_read;                    // increment the number of bytes received
                buffer->add(local_buffer, bytes_read);
            }

        }
    }
    catch (SocketException &se) {
		UNREFERENCED_PARAMETER(se);
        delete[] local_buffer;
        throw;
    }
    catch (TimeoutException &te) {
		UNREFERENCED_PARAMETER(te);
        delete[] local_buffer;
        throw;
    }

    // deallocate the local buffer
    delete[] local_buffer;

    return alive_;
}

bool TcpConnection::send_data(std::shared_ptr<SendSocketBuffer> buffer) const {
    const auto total_bytes = buffer->get_size();
	auto sent_bytes = 0;

    while (buffer->get_bytes_sent() < total_bytes) {

        sent_bytes = send(sock_, buffer->get_buffer(), buffer->get_remaining_bytes(), 0);

        if (sent_bytes == 0) return false;

        if (sent_bytes == SOCKET_ERROR) throw SocketException(WSAGetLastError());

        buffer->send(sent_bytes);
    }

    return true;
}


bool TcpConnection::read_line(std::shared_ptr<SocketBuffer> buffer) const {

    // TODO ricordarsi di controllare se il buffer finisce con \r\n
	auto read_byte = 0;
    auto *local_buffer = new char[buffer->get_max_size()];
    memset(local_buffer, 0, static_cast<std::size_t>(buffer->get_max_size()));

    struct timeval time;
    FD_SET read_sock;

    FD_ZERO(&read_sock);
    FD_SET(sock_, &read_sock);

    time.tv_sec = SEC_;
    time.tv_usec = USEC_;

    const auto result = select(sock_ + 1, &read_sock, nullptr, nullptr, &time);

    if (result == SOCKET_ERROR) throw SocketException(WSAGetLastError());

    if (result == 0) throw TimeoutException();

    if (result > 0) read_byte = readline_unbuffered(local_buffer, buffer->get_max_size());

    if (read_byte == 0) return false;

    if (read_byte < 0) throw SocketException(WSAGetLastError());

    buffer->add(local_buffer, read_byte);
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

    if (result > 0) return recv(sock_, read_buffer, size, 0);

	return 0;
}
