#include "Server.hpp"

/*
 * this constructor create a new socket that is setted on the defaul port 1500;
 * if you want to change it is possible to call the method change port and pass the new port number, t
 */
Server::Server() : socket(DEFAULT_LISTEN_PORT)
{
	// when a new instance of Server is declared, a new listen socket is created  and binded to receive incoming request
	// winsock startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		std::cout << "WinSock startup fail" << std::endl;
		exit(1);
	}

	server_state = CREATED;
	isPaused = false;
	isStopped = false;

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

Server::~Server()
{
	closeServer();
}

void Server::runServer() 
{

	if(server_state != CREATED) {
		return;
	}
	
	while (!isStopped) {
		server_state == RUNNING;
		std::cout << "the server is running" << std::endl;

		while (!isPaused || !isStopped) {
			listenNewConnection();
		}

		server_state = PAUSED;
		std::cout << "the server is paused" << std::endl;
		std::unique_lock<std::mutex> ul(mtx);
		cv.wait(ul);
	}
		
	server_state = STOPPED;
	std::cout << "the server is stopped" << std::endl;
}

void Server::listenNewConnection() 
{
	connection::TCPConnection newConn;
	std::time_t timestamp;
			
	// accept an imcoming request
	if (newConn.accept_connection(socket))
	{	
		// print some informations about the remote end-point, the time stamp when the connection has been accepted and more...
		timestamp = std::time(0);
		std::cout << "***************************************************" << std::endl;
		std::cout << "(" << std::put_time(std::localtime(&timestamp), "%c") << ") ";
		std::cout << "Server accepted an incoming request" << std::endl << "Client information: " << std::endl;
		newConn.print_endpoint_info();
		std::cout << "***************************************************" << std::endl;
		
		// add the request inside the request manager
		request_status status;
		if (request_manager->addConnection(std::make_shared<connection::TCPConnection>(newConn), status)) 
		{
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
	else 
	{
		// TODO	notify to the user, via user interface, the connection problem
	}
}

bool Server::changePort(int port)
{
	// create a new instance of the listen_socket only if the server is
	if (port >= 0 && port <= MAX_PORT && server_state == CREATED)
	{
		socket = Listen_socket(port);
		return true;
	}

	return false;
}

void Server::startServer() 
{
	// re-instantiate the request manager and the download manager
	download_manager = std::shared_ptr<DownloadManager>(new DownloadManager());
	request_manager = std::shared_ptr<RequestManager>(new RequestManager(download_manager));
	
	// start a new thread
	server_main_thread = std::thread(&Server::runServer, this);
}

void Server::restartServer() 
{
	closeServer();
	server_state = CREATED;
	isPaused = false;
	isStopped = false;
	startServer();
}

void Server::pauseServer()
{
	if(server_state == RUNNING)
	{
		isPaused = true;
	}
}

void Server::rerunServer()
{
	if(server_state == PAUSED)
	{
		isPaused = false;
		std::lock_guard<std::mutex> lock_guard(mtx);
		cv.notify_all();
	}

	if (server_state == STOPPED)
	{
		std::cout << "the server is stopped, is not possible to stop it" << std::endl;
	}
}

void Server::closeServer()
{
	if(server_state == RUNNING || server_state == PAUSED)
	{
		isStopped = true;
		std::lock_guard<std::mutex> lock_guard(mtx);
		cv.notify_all();
	}

	closesocket(socket.getSocket());

	if(server_state == STOPPED)
	{
		server_main_thread.join();
	}

	// delete the pointer to the old managers
	// since the request manager has associated a pointer to the downlaod manager, should be resetted first the request manager pointer 
	// and then the download manager, otherwise the shared pointer is pointed two times and the instance is never destructed correctly
	request_manager.reset();
	download_manager.reset();
}
