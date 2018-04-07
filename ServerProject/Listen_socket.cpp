#include "Listen_socket.h"

/*
 * this constructor create a new non connected socket used by the server to accept the incoming tcp connection
 */
Listen_socket::Listen_socket(int port) 
{
	// create the data structure that contain the local address and the port of the server
	local_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr_.sin_port = htons(port);
	local_addr_.sin_family = AF_INET;

	l_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (bind(l_socket_, (SOCKADDR*)&local_addr_, sizeof(local_addr_)) == SOCKET_ERROR) {
		std::cout << "impossible to bind the socket with the specified address, error:" << std::to_string(WSAGetLastError()) << std::endl;
		exit(1);
	}
	if (listen(l_socket_, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "server cannot listen for incoming request, error: " << std::to_string(WSAGetLastError()) << std::endl;
		exit(1);
	}
}

/*
 * copy constructor of the listen socket class
 */
Listen_socket::Listen_socket(const Listen_socket& socket)
{
	this->l_socket_ = socket.l_socket_;
	this->local_addr_ = socket.local_addr_;
	this->server_port_ = socket.server_port_;
}

/*
 * assing operator of the listen socket class
 */
Listen_socket& Listen_socket::operator=(const Listen_socket& socket)
{
	if(this != &socket)
	{
		this->l_socket_ = socket.l_socket_;
		this->local_addr_ = socket.local_addr_;
		this->server_port_ = socket.server_port_;
	} 
	return *this;
}

/*
 * return the socket
 */
SOCKET Listen_socket::get_socket()
{
	return l_socket_;
}

Listen_socket::~Listen_socket()
{
}
