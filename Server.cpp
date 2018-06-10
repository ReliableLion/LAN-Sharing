
#pragma warning(disable:4996)

#include "Server.hpp"
#include <iostream>
#include "ProtocolMessageTest.hpp"

/*
* this constructor create a new socket that is setted on the defaul port 1500;
* if you want to change it is possible to call the method change port and pass the new port number, t
*/
Server::Server(const int port): server_status_() {

	if (port < 0 || port > 65535) {
		std::cout << "the value of the port passed as parameter is not valid" << std::endl;
		exit(1);
	}

	// create the data structure that contain the local address and the server port
	local_address_.sin_addr.s_addr = htonl(INADDR_ANY);
	local_address_.sin_port = htons(static_cast<uint16_t>(port));
	local_address_.sin_family = AF_INET;

	passive_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (bind(passive_socket_, reinterpret_cast<SOCKADDR *>(&local_address_), sizeof(local_address_)) == SOCKET_ERROR) {
		std::cout << "impossible to bind the socket with the specified address, error:"
			<< std::to_string(WSAGetLastError()) << std::endl;
		exit(1);
	}

	if (listen(passive_socket_, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "server cannot listen for incoming request, error: "
			<< std::to_string(WSAGetLastError())
			<< std::endl;
		exit(1);
	}

	// conver the address from sockaddr type into a human readable format
	char address_msg[1024];
	inet_ntop(AF_INET, &(local_address_.sin_addr), address_msg, 1024);

	std::cout << "this server has address: " << address_msg << std::endl;
	std::cout << "port number: " << ntohs(local_address_.sin_port) << std::endl << std::endl;
}

Server::~Server() {
    close_server();
}

void Server::run_server() {

    std::cout << "the server is running" << std::endl << std::endl;

    while (server_status_ == running) {
        listen_new_connection();
    }
}

void Server::print_client_info(const std::chrono::time_point<std::chrono::system_clock> time_point,
                               connection::TcpConnection &connection) {
    auto time_stamp_t = std::chrono::system_clock::to_time_t(time_point);

    std::cout << "***************************************************" << std::endl;
    std::cout << "(" << std::put_time(std::localtime(&time_stamp_t), "%c") << ") ";
    std::cout << "Server accepted an incoming request" << std::endl << "Client information: " << std::endl;
    connection.print_endpoint_info();
    std::cout << "***************************************************" << std::endl;
}

void Server::listen_new_connection() {
	SOCKADDR_IN client_address;
    int addrlen = sizeof(client_address);

    // accept an imcoming request
	const auto accept_socket = accept(passive_socket_, reinterpret_cast<SOCKADDR *>(&client_address), &addrlen);

    if (WSAGetLastError() == WSAEINVAL) {
        server_status_ = stopped;
        return;
    }

    connection::TcpConnection new_connection(accept_socket, client_address);

    // print some informations about the remote end-point, the time stamp when the connection has been accepted and more...
    print_client_info(std::chrono::system_clock::now(), new_connection);



    // TODO da rimuovere dopo aver fatto il test
//    ProtocolMessageTest proto;
//    proto.download_request(std::make_shared<connection::TcpConnection>(new_connection));


     //add the request inside the request manager
	request_status status;
	if (request_manager_->add_connection(std::make_shared<connection::TcpConnection> (new_connection), status))
	{
		switch (status)
		{
		case full_queue :
			std::cout << "impossible to add the connection, the queue is full" << std::endl;
			break;
		case term_signal :
			std::cout << "impossibleto add the connection becase the request manager hase reveived the shutdown request" << std::endl;
			break;
		}
	}
}

void Server::start_server() {

	download_manager_ = std::make_shared<DownloadManager>();
    request_manager_ = std::make_shared<RequestManager>(download_manager_);

    server_status_ = running;
    server_main_thread_ = std::thread(&Server::run_server, this);
}

void Server::close_server() {

    closesocket(passive_socket_);
    server_main_thread_.join();

    // delete the pointer to the old managers
    // since the request manager has associated a pointer to the downlaod manager, should be resetted first the request manager pointer
    // and then the download manager, otherwise the shared pointer is pointed two times and the instance is never destructed correctly

    request_manager_.reset();
    download_manager_.reset();
}
