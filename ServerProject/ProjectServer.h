#ifndef SERVER_H
#define SERVER_H 

#include <iostream>
#include <thread>
#include "RequestMananger.h"
#include "UserConfiguration.h"

#include <boost/asio.hpp>

using namespace boost::asio;

/*
	this class should wait for the incoming request from the client 
*/
class Server {
private:
	io_service io;
	const std::string ServerPort = "1500";
	const std::string ServerAddr = "localhost";
	ip::tcp::acceptor Acceptor;

	UserConfig AppConfiguration;
	RequestManager reqMan;

	void _waitRequest();
	void _handleAccept(const boost::system::error_code& e, std::shared_ptr<TCPconnection> conn);

public:
	// definire il construttore dopo perchè bisonga tenere conto dell'indirizzo del server e della porta
	Server();
	Server(const std::string ServerAddr, const std::string ServerPort);
};

#endif // !SERVER_H
