#include "stdafx.h"

#pragma warning(disable:4996)

#include "Server.hpp"

/*
* this constructor create a new socket that is setted on the defaul port 1500;
* if you want to change it is possible to call the method change port and pass the new port number, t
*/
Server::Server(int port) {
    // TODO da cambiare l'eccezione
    if (port < 0 || port > 65535) throw SocketException(1);

    // create the data structure that contain the local address and the port of the server
    local_address_.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address_.sin_port = htons((uint16_t) port);
    local_address_.sin_family = AF_INET;

    passive_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (bind(passive_socket_, (SOCKADDR *) &local_address_, sizeof(local_address_)) == SOCKET_ERROR) {
        std::cout << "impossible to bind the socket with the specified address, error:"
                  << std::to_string(WSAGetLastError()) << std::endl;
        exit(1);
    }

    if (listen(passive_socket_, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "server cannot listen for incoming request, error: " << std::to_string(WSAGetLastError())
                  << std::endl;
        exit(1);
    }

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

    while (server_status == RUNNING) {
        listen_new_connection();
    }
}

void Server::print_client_info(std::chrono::time_point<std::chrono::system_clock> time_point,
                               connection::TCPConnection &connection) {
    auto time_stamp_t = std::chrono::system_clock::to_time_t(time_point);

    std::cout << "***************************************************" << std::endl;
    std::cout << "(" << std::put_time(std::localtime(&time_stamp_t), "%c") << ") ";
    std::cout << "Server accepted an incoming request" << std::endl << "Client information: " << std::endl;
    connection.print_endpoint_info();
    std::cout << "***************************************************" << std::endl;
}

void Server::listen_new_connection() {
    SOCKET accept_socket;
    SOCKADDR_IN client_address;
    int addrlen = sizeof(client_address);

    // accept an imcoming request
    accept_socket = accept(passive_socket_, (SOCKADDR *) &client_address, &addrlen);

    if (WSAGetLastError() == WSAEINVAL) {
        server_status = STOPPED;
        return;
    }

    connection::TCPConnection new_connection(accept_socket, client_address);

    // print some informations about the remote end-point, the time stamp when the connection has been accepted and more...
    print_client_info(std::chrono::system_clock::now(), new_connection);



    // TODO da rimuovere dopo aver fatto il test
    ProtocolMessageTest proto;
    proto.download_request(std::make_shared<connection::TCPConnection>(new_connection));


    // add the request inside the request manager
//	request_status status;
//	if (request_manager_->add_connection(std::make_shared<connection::TCPConnection> (new_connection), status))
//	{
//		switch (status)
//		{
//		case FULL_QUEUE:
//			std::cout << "impossible to add the connection, the queue is full" << std::endl;
//			break;
//		case TERM_SIGNAL:
//			std::cout << "impossibleto add the connection becase the request manager hase reveived the shutdown request" << std::endl;
//			break;
//		}
//	}
}

void Server::start_server() {
    // TODO cambiare in make shared
    download_manager_ = std::shared_ptr<DownloadManager>(new DownloadManager());
    request_manager_ = std::shared_ptr<RequestManager>(new RequestManager(download_manager_));

    server_status = RUNNING;
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
