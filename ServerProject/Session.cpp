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
	if (closesocket(sock) == SOCKET_ERROR) {
		std::cout << "problem during the socket closing, error: " << std::to_string(WSAGetLastError()) << std::endl;
		exit(1);
	} else {
		std::cout << "connection closed" << std::endl;
	}
}

void session::TCPConnection::printEndpointInfo() {
	if (sock == 0) {
		std::cout << "socket not connected" << std::endl;
		return;
	}
	std::cout << "IP address: " << remote_address.sin_addr.s_addr << std::endl;
	std::cout << "port number: " << remote_address.sin_port << std::endl << std::endl;
}

bool session::TCPConnection::recvall(char* data, int totalByte) {
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
				return false;
			}
			else {
				byteReceived += byteRead;
			}

		} else {
			return false;
		}
	}
	std::cout << "data received correctly!" << std::endl;
	return true;
}

bool session::TCPConnection::sendall(const char* data, int totalByte) {
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