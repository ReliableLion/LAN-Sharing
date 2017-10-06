#include "ProjectServer.h"
#include <boost\asio.hpp>
#include <iostream>

using namespace boost::asio;

// this function wait to receive an inbound request from the client of the application 
void Server::WaitRequest() {


	try {
		
		// iterate if the configuration of the app is setted to PUBLIC
		while (AppConfiguration.get_isPublic()) {

			//  the end point is specified as an IPv4 address on port 1500; 
			ip::tcp::acceptor accept(io_service, ip::tcp::endpoint(ip::tcp::v4(), ServerPort));

			// creation of new connection 

		}

	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}




}



