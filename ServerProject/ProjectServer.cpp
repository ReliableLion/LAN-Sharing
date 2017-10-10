#include "ProjectServer.h"

/*
	create a new server instance and resolve the endpoint name 
*/
Server::Server(const std::string ServerAddr, const std::string ServerPort) {

	ip::tcp::acceptor Acceptor = ip::tcp::acceptor(io);

	ip::tcp::resolver resolver(io);
	ip::tcp::resolver::query query(ServerAddr, ServerPort);
	ip::tcp::endpoint server_endpoint = *resolver.resolve(query);

	Acceptor.open(server_endpoint.protocol);
	Acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
	Acceptor.bind(server_endpoint);
	Acceptor.listen();

	// start to wait for new request
	_waitRequest();
}


/*
	create a new server instance using default configuration
*/
Server::Server() {

	ip::tcp::acceptor Acceptor = ip::tcp::acceptor(io);

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


// this function wait to receive an inbound request from the client of the application 
void Server::_waitRequest() {


	try {
		
		// iterate if the configuration of the app is setted to PUBLIC
		while (AppConfiguration.get_isPublic()) {

			//  the end point is specified as an IPv4 address on port 1500; 
			//ip::tcp::acceptor accept(io_service, ip::tcp::endpoint(ip::tcp::v4(), ServerPort);

			// creation of new connection 

		}

	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}



