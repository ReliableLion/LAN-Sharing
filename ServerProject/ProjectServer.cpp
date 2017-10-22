#include "ProjectServer.h"
#include <boost\bind.hpp>

/*
	create a new server instance and resolve the endpoint name 
*/
Server::Server(std::string ServerAddr, int  ListenPort) : io(), Acceptor(io), reqMan() {
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
}

/*
	create a new server instance using default configurations
*/
Server::Server() : io(), Acceptor(io), reqMan() {
	ip::tcp::resolver resolver(io);
	local_endpoint = ip::tcp::endpoint(ip::tcp::v4(), int_ServerPort);
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

	try {
		// iterate if the configuration of the app is setted to PUBLIC
		while (AppConfiguration.get_isPublic()) {

			//  the end point is specified as an IPv4 address on port 1500; 
			// ip::tcp::acceptor accept(io_service, ip::tcp::endpoint(ip::tcp::v4(), ServerPort);
			// creation of new connection 

			std::shared_ptr<TCPconnection_server> newConnection = std::shared_ptr<TCPconnection_server>(new TCPconnection_server(io));

			// accept  request
			Acceptor.accept(newConnection->getSocket());
			reqMan.addRequest(newConnection);
		}

		// close all the connection 
		reqMan.shutdown();
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