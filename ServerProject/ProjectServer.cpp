#include "ProjectServer.h"
#include <boost\bind.hpp>

/*
	create a new server instance and resolve the endpoint name 
*/
Server::Server(std::string ServerAddr, int  ListenPort) : io(), Acceptor(io), 
		d_man_ptr(std::shared_ptr<DownloadManager> (new DownloadManager())), reqMan(d_man_ptr) {

	this->ServerAddr = ServerAddr;
	this->int_ServerPort = ListenPort;
	this->ServerPort = std::to_string(ListenPort);

	if (_portChecking(int_ServerPort)) {
		std::cout << "the port specified is not correct" << std::endl;
		throw std::exception();
	}

	ip::tcp::resolver resolver(io);
	ip::tcp::resolver::query query(this->ServerAddr, this->ServerPort);
	local_endpoint = *resolver.resolve(query);
	d_man_ptr = std::shared_ptr<DownloadManager>(new DownloadManager());
}

/*
	create a new server instance using default configurations
*/
Server::Server() : io(), Acceptor(io), 
		d_man_ptr(std::shared_ptr<DownloadManager> (new DownloadManager())), reqMan(d_man_ptr) {
	
	ip::tcp::resolver resolver(io);
	local_endpoint = ip::tcp::endpoint(ip::tcp::v4(), int_ServerPort);
	d_man_ptr = std::shared_ptr<DownloadManager>(new DownloadManager());
}

Server::~Server() {
	reqMan.shutdown();
	d_man_ptr->exitDownloader();
}

/*
	throw an exception if the Server receive as actual parameter a not valid port number
*/
void Server::createServer() {
	Acceptor.open(local_endpoint.protocol());
	Acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
	Acceptor.bind(local_endpoint);
	Acceptor.listen();

	// begin to wait for request
	_waitRequest();
}

void Server::closeServer() {
	// close all possible instance declarated into server class
	Acceptor.close();
	reqMan.shutdown();
}

// this function wait an inbound request from the applcation client
void Server::_waitRequest() {
	std::time_t timestamp;

	try {
		// iterate if the configuration of the app is setted to PUBLIC
		while (AppConfiguration.get_isPublic()) {
			//? is better to instantiate a new connection before or after the accept?
			std::shared_ptr<TCPconnection_server> newConnection = std::shared_ptr<TCPconnection_server>(new TCPconnection_server(io));

			// print that the server is waiting for incoming request
			std::cout << "Server on IP address: " << local_endpoint.address() << " and port: " << local_endpoint.port() <<  " wait for incoming request..." << std::endl;

			// accept  request
			Acceptor.accept(newConnection->getSocket());
			ip::tcp::endpoint remote_endpoint = newConnection->getRemoteEndpoint();
			timestamp = std::time(nullptr);

			//? is the code below necessary for the application?
			// connection info 
			std::cout << "(" << std::asctime(std::localtime(&timestamp)) << ")" << " server accepted an incoming request:" << std::endl;
			std::cout << "address: "  << remote_endpoint.address() << std::endl;
			std::cout << "port: " << remote_endpoint.port() << std::endl << std::endl;

			reqMan.addRequest(newConnection);
		}

		// close all the connection 
		reqMan.shutdown();
		reqMan.closeConnections();
		d_man_ptr->exitDownloader();
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}


/*
	check if the port number is  between 0 and 65535
	return false if the int is not valid
*/
bool Server::_portChecking(int port_number) {

	if (port_number > MAX_PORT_N || port_number < MIN_PORT_N) {
		return false;
	}
	else {
		return true;
	}

}