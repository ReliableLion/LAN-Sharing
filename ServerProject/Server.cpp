#include "Server.hpp"

//			PRIVATE METHODS




//			PUBLIC METHODS

Server::Server(int port = 1500){
	// when a new instance of Server is declared, a new listen socket is created  and binded to receive incoming request
	// winsock startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		std::cout << "WinSock startup fail" << std::endl;
		exit(1);
	}

	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(port);
	local_addr.sin_family = AF_INET;

	l_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(l_socket, (SOCKADDR*) &local_addr, sizeof(local_addr)) == SOCKET_ERROR) {
		std::cout << "impossible to bind the socket with the specified address, error:" << std::to_string(WSAGetLastError()) << std::endl;
		exit(1);
	}
	if (listen(l_socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "server cannot listen for incoming request, error: " << std::to_string(WSAGetLastError()) << std::endl;
		exit(1);
	}
}

Server::~Server() {
	
}

void Server::listenNewConnection() {
	// accept the incoming request 
	session::TCPConnection newConn;
	if (newConn.acceptConnection(l_socket)) {
		newConn.printEndpointInfo();
	}
	else {
		// TODO	notify to the user, via user interface, the connection problem

	}
}