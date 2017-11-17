#include "Server.hpp"

//			PRIVATE METHODS




//			PUBLIC METHODS

Server::Server(int port){
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
	session::TCPConnection newConn;
	std::time_t timestamp;
												
	if (newConn.acceptConnection(l_socket)) {				// accept an imcoming request
		timestamp = std::time(0);
		std::cout << "***************************************************" << std::endl;
		std::cout << "(" << std::put_time(std::localtime(&timestamp), "%c") << ") ";
		std::cout << "Server accepted an incoming request" << std::endl << "Client information: " << std::endl;
		newConn.printEndpointInfo();
		std::cout << "***************************************************" << std::endl;
	}
	else {
		// TODO	notify to the user, via user interface, the connection problem

	}
}