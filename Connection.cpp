#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace connection;
using namespace std::chrono;

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

	// allocate a reading buffer
	reading_buffer = new char[CHUNK];
	// allocate a new buffer
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
	bytes_read = recv(sock_, buffer->write_to_buffer(), buffer->get_max_size(), 0);

	if (bytes_read != 1460)
		std::cout << "djaosdiaj";

	if (bytes_read == SOCKET_ERROR) {
		std::cout << WSAGetLastError() << endl;
		throw SocketException(WSAGetLastError());
	}
	else if (bytes_read == 0) {
		alive_ = false;
		return false;
	} else if (bytes_read == 1 && buffer->read_from_buffer()[0] == '\0') {
		std::cout << "the receive return an empty buffer" << std::endl;
		return false;
	} else {
		buffer->bytes_wrote(bytes_read);
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
        bytes_sent = send(sock_, buffer->read_from_buffer(), buffer->get_remaining_bytes(), 0);

		if (bytes_sent == 0) {
			alive_ = false;
			return false;
		} else if (bytes_sent == SOCKET_ERROR) 
			throw SocketException(WSAGetLastError());

		buffer->bytes_read(bytes_sent);
    }

    return true;
}

bool TcpConnection::read_line(std::shared_ptr<SocketBuffer> buffer) {

	select_connection();

	ZeroMemory(receive_buffer_, 0, CHUNK);
	int read_bytes = readline_unbuffered(receive_buffer_, buffer->get_max_size());

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

void TcpConnection::select_connection() {
    struct timeval time;
    FD_SET read_sock;

    FD_ZERO(&read_sock);
    FD_SET(sock_, &read_sock);

    time.tv_sec = SEC_;
    time.tv_usec = USEC_;

    const int result = select(sock_ + 1, &read_sock, nullptr, nullptr, &time);

    if (result == SOCKET_ERROR) 
		throw SocketException(WSAGetLastError());
    else if (result == 0) 
		throw TimeoutException();
}

void TcpConnection::select_write_connection() {

    struct timeval time;
    FD_SET write_sock;

    FD_ZERO(&write_sock);
    FD_SET(sock_, &write_sock);

    time.tv_sec = SEC_;
    time.tv_usec = USEC_;

    const int result = select(sock_ + 1, nullptr, &write_sock, nullptr, &time);

    if (result == SOCKET_ERROR) 
		throw SocketException(WSAGetLastError());
    else if (result == 0) 
		throw TimeoutException();
}

int TcpConnection::receive_file(size_t file_size, FileHandler &temporary_file, std::string requestID) {

	signed long long int left_bytes = file_size;
	signed long long int bytes_to_download = 0, total_downloaded_bytes = 0, threshold = 0, percentage_limit = 0;
	int percentage = 0;
	bool connection_closed = false;

	std::shared_ptr<SocketBuffer> buffer = std::make_shared<SocketBuffer>();
	const auto buffer_max_size = buffer->get_max_size();

	select_connection();

	temporary_file.open_file(write);										// open the two files, if an exception is throw by the program then the file is closed by the destructor

	// auto start = high_resolution_clock::now();

	// compute the threshold 
	threshold = (left_bytes * 10) / 100;

	while (left_bytes != 0 && alive_) {

		if (left_bytes >= buffer_max_size) {								// if the remaining data are greater than the max size of the buffer then the bytes to download are max buff lengh
			bytes_to_download = buffer_max_size;
		}
		else {
			bytes_to_download = left_bytes;									// if the remaining data are smaller than the max, set the remaining bytes value
		}

		if (read_data(buffer)) {
			left_bytes -= buffer->get_size();
			temporary_file.write_data(buffer);

			total_downloaded_bytes += buffer->get_size();

			if (total_downloaded_bytes > percentage_limit) {
				if (percentage < 100) {
					percentage += 10;
					managed_callback::getInstance().call_progress_bar_callback(requestID.c_str(), percentage);
					percentage_limit += threshold;
				}
			}
		}
	}

	if (left_bytes == 0)
		managed_callback::getInstance().call_progress_bar_callback(requestID.c_str(), 100);
	else
		managed_callback::getInstance().call_progress_bar_callback(requestID.c_str(), percentage);

	/*
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);

	std::cout << "Time taken by function: "
		<< duration.count() << " microseconds" << std::endl; */

	temporary_file.close_file();
	return left_bytes;
}

int TcpConnection::send_file(HANDLE file_handle, DWORD file_size, string requestID) {

	signed long long int left_bytes = file_size, bytes_sent = 0, byte_to_read = 0, total_byte_sent = 0, percentage_limit = 0, treshold = 0;
	int percentage = 0;
	DWORD bytes_read = 0;

	select_write_connection();

	SocketBuffer socket_buffer;

	if (left_bytes < CHUNK)
		byte_to_read = static_cast<long>(left_bytes);
	else
		byte_to_read = static_cast<long>(CHUNK);

	treshold = left_bytes/10;
	//total_byte_sent += treshold;

	while(left_bytes > 0) {

		if(!ReadFile(file_handle, socket_buffer.write_to_buffer(), byte_to_read, &bytes_read, nullptr)) {
			std::cout << "Some errors occurs while reading: " << GetLastError() << std::endl;
			close_connection();
			return left_bytes;
		}

		socket_buffer.bytes_wrote(static_cast<int>(bytes_read));

		while(socket_buffer.get_remaining_bytes() != 0) {

			if((bytes_sent = send(sock_, socket_buffer.read_from_buffer(), socket_buffer.get_remaining_bytes(), 0)) == SOCKET_ERROR) {
				throw SocketException(WSAGetLastError());
			}
			if(bytes_sent == 0){
				std::cout << "Connection closed by peer." << std::endl;
				return 0;
			}

			socket_buffer.bytes_read(bytes_sent);
			total_byte_sent += bytes_sent;
		}

		if(total_byte_sent > percentage_limit) {
			if(percentage < 100) {
				percentage += 10;
				managed_callback::getInstance().call_progress_bar_callback(requestID.c_str(), percentage);
				percentage_limit += treshold;
			}
		}

		left_bytes -= bytes_sent;

        if (left_bytes < CHUNK)
			byte_to_read = static_cast<long>(left_bytes);
	}

	if(left_bytes == 0)
		managed_callback::getInstance().call_progress_bar_callback(requestID.c_str(), 100);
	else
		managed_callback::getInstance().call_progress_bar_callback(requestID.c_str(), percentage);

	return left_bytes;
}

bool TcpConnection::get_connection_status() { return alive_; }

bool TcpConnection::read_n_data(std::shared_ptr<SocketBuffer> buffer, int n) { return true; }
