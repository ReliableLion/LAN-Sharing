#include "ProjectServer.h"
#include <boost\bind.hpp>

/*
	create a new server instance and resolve the endpoint name 
*/
Server::Server(std::string ServerAddr, int  ListenPort) : io(), Acceptor(io), reqMan() {

	this->ServerAddr = ServerAddr;
	this->ServerPort = std::to_string(ListenPort);

	ip::tcp::resolver resolver(io);
	ip::tcp::resolver::query query(ServerAddr, ServerPort);
	ip::tcp::endpoint server_endpoint = *resolver.resolve(query);

	Acceptor.open(server_endpoint.protocol());
	Acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
	Acceptor.bind(server_endpoint);
	Acceptor.listen();

	// wait for new request
	_waitRequest();
}


/*
	create a new server instance using default configurations
*/
Server::Server() : io(), Acceptor(io), reqMan() {

	ip::tcp::resolver resolver(io);
	ip::tcp::resolver::query query(this->ServerAddr, this->ServerPort);
	ip::tcp::endpoint server_endpoint = *resolver.resolve(query);

	Acceptor.open(server_endpoint.protocol());
	Acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
	Acceptor.bind(server_endpoint);
	Acceptor.listen();

	// begin to wait for request
	_waitRequest();
}


// this function wait an inbound request from the applcation client
void Server::_waitRequest() {

	try {
		// iterate if the configuration of the app is setted to PUBLIC
		while (AppConfiguration.get_isPublic()) {

			//  the end point is specified as an IPv4 address on port 1500; 
			// ip::tcp::acceptor accept(io_service, ip::tcp::endpoint(ip::tcp::v4(), ServerPort);
			// creation of new connection 

			std::shared_ptr<TCPconnection> newConnection = std::shared_ptr<TCPconnection>(new TCPconnection(io));
		}

		// TODO close all the connections
		//connMan.stop_all();

	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

void Server::closeServer() {
	// close all possible instance declarated into server class
	Acceptor.close();
	reqMan.shutdown();
}




