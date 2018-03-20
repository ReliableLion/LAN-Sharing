#include "Listen_socket.h"

/*
 * this constructor create a new non connected socket used by the server to accept the incoming tcp connection
 */
Listen_socket::Listen_socket(int port) 
{
	// create the data structure that contain the local address and the port of the server
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(port);
	local_addr.sin_family = AF_INET;

	l_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (bind(l_socket, (SOCKADDR*)&local_addr, sizeof(local_addr)) == SOCKET_ERROR) {
		std::cout << "impossible to bind the socket with the specified address, error:" << std::to_string(WSAGetLastError()) << std::endl;
		exit(1);
	}
	if (listen(l_socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "server cannot listen for incoming request, error: " << std::to_string(WSAGetLastError()) << std::endl;
		exit(1);
	}
}

/*
 * copy constructor of the listen socket class
 */
Listen_socket::Listen_socket(const Listen_socket& socket)
{
	this->l_socket = socket.l_socket;
	this->local_addr = socket.local_addr;
	this->server_port = socket.server_port;
}

/*
 * assing operator of the listen socket class
 */
Listen_socket& Listen_socket::operator=(const Listen_socket& socket)
{
	if(this != &socket)
	{
		this->l_socket = socket.l_socket;
		this->local_addr = socket.local_addr;
		this->server_port = socket.server_port;
	} 
	return *this;
}

/*
 * return the socket
 */
SOCKET Listen_socket::getSocket()
{
	return l_socket;
}

Listen_socket::~Listen_socket()
{
}
