#include "ProjectServer.h"
#include <boost\bind.hpp>

/*
	create a new server instance and resolve the endpoint name 
*/
Server::Server(const std::string ServerAddr, const std::string ServerPort) : io(), Acceptor(io), connMan() {

	ip::tcp::resolver resolver(io);
	ip::tcp::resolver::query query(ServerAddr, ServerPort);
	ip::tcp::endpoint server_endpoint = *resolver.resolve(query);

	Acceptor.open(server_endpoint.protocol);
	Acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
	Acceptor.bind(server_endpoint);
	Acceptor.listen();

	// initialize the downloder
	d_handler.setupDownloader();

	// wait for new request
	_waitRequest();
}


/*
	create a new server instance using default configurations
*/
Server::Server() : io(), Acceptor(io), connMan() {

	ip::tcp::resolver resolver(io);
	ip::tcp::resolver::query query(this->ServerAddr, this->ServerPort);
	ip::tcp::endpoint server_endpoint = *resolver.resolve(query);

	Acceptor.open(server_endpoint.protocol());
	Acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
	Acceptor.bind(server_endpoint);
	Acceptor.listen();

	// initialize the download handler
	d_handler.setupDownloader();

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

			std::shared_ptr<TCPconnection> newConnection = std::shared_ptr<TCPconnection>(new TCPconnection());
			newConnection->createConnection(io);

			//async accept 
			Acceptor.async_accept(newConnection->getSocket(), boost::bind(&Server::_handleAccept, this, boost::asio::placeholders::error));

		}

		// TODO close all the connection


	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}


void Server::_handleAccept(const boost::system::error_code& e, std::shared_ptr<TCPconnection> conn) {

	if (!Acceptor.is_open()) {
		return;
	}

	
	if (!e) {
		connMan.start(conn);
	}
	else {
		// error
		std::cerr << "" << std::endl;
	}
}



