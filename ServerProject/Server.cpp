#include "Server.hpp"


/*
 * this constructor create a new socket that is setted on the defaul port 1500;
 * if you want to change it is possible to call the method change port and pass the new port number, t
 */
Server::Server() : socket(DEFAULT_LISTEN_PORT) {
	// when a new instance of Server is declared, a new listen socket is created  and binded to receive incoming request
	// winsock startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		std::cout << "WinSock startup fail" << std::endl;
		exit(1);
	}

	server_state = CREATED;

	// create an instance of the download and request manager
	download_manager = std::shared_ptr<DownloadManager>(new DownloadManager());
	request_manager = std::shared_ptr<RequestManager>(new RequestManager(download_manager));

	/*local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(port);
	local_addr.sin_family = AF_INET;

	l_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (bind(l_socket, (SOCKADDR*) &local_addr, sizeof(local_addr)) == SOCKET_ERROR) {
		std::cout << "impossible to bind the socket with the specified address, error:" << std::to_string(WSAGetLastError()) << std::endl;
		exit(1);
	}
	if (listen(l_socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "server cannot listen for incoming request, error: " << std::to_string(WSAGetLastError()) << std::endl;
		exit(1);
	}*/

}

Server::~Server() {
	closeServer();
}

void Server::runServer() {
	bool signal_exit = true;

	if(server_state == STOPPED)
	{
		// re-instantiate the request manager and the download manager
		download_manager = std::shared_ptr<DownloadManager>(new DownloadManager());
		request_manager = std::shared_ptr<RequestManager>(new RequestManager(download_manager));
		server_state = RUNNING;
	} 
	
	server_state = RUNNING;
	std::cout << "the server is running" << std::endl;

	// TODO important: create a communication mechanism between UI and server thread 
	while (signal_exit)
	{
		listenNewConnection();
	}

	server_state = STOPPED;
	std::cout << "the server has been stopped by an external signal" << std::endl;

	// delete the pointer to the old managers
	// since the request manager has associated a pointer to the downlaod manager, should be resetted first the request manager pointer 
	// and then the download manager, otherwise the shared pointer is pointed two times and the instance is never destructed correctly
	request_manager.reset();
	download_manager.reset();
}

void Server::listenNewConnection() {
	connection::TCPConnection newConn;
	std::time_t timestamp;
			
	// accept an imcoming request
	if (newConn.accept_connection(socket)) {	
		
		// print some informations about the remote end-point, the time stamp when the connection has been accepted and more...
		timestamp = std::time(0);
		std::cout << "***************************************************" << std::endl;
		std::cout << "(" << std::put_time(std::localtime(&timestamp), "%c") << ") ";
		std::cout << "Server accepted an incoming request" << std::endl << "Client information: " << std::endl;
		newConn.print_endpoint_info();
		std::cout << "***************************************************" << std::endl;
		
		// add the request inside the request manager
		request_status status;
		if (request_manager->addConnection(std::make_shared<connection::TCPConnection>(newConn), status)) {
			switch(status)
			{
			case FULL_QUEUE:
				std::cout << "impossible to add the connection, the queue is full" << std::endl;
				break;
			case TERM_SIGNAL:
				std::cout << "impossibleto add the connection becase the request manager hase reveived the shutdown request" << std::endl;
				break;
			}
		}
	}
	else {
		// TODO	notify to the user, via user interface, the connection problem
	}
}

bool Server::changePort(int port)
{
	if (port >= 0 && port <= MAX_PORT)
	{
		// create a new listen socket with a new port
		socket = Listen_socket(port);
		return true;
	}

	return false;
}

void Server::closeServer()
{
	closesocket(socket.getSocket());						// close the socket 
}
