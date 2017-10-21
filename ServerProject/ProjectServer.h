#ifndef SERVER_H
#define SERVER_H 

#include <iostream>
#include <thread>
#include <string>
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
	std::string ServerPort = "1500";
	std::string ServerAddr = "localhost";
	ip::tcp::acceptor Acceptor;

	UserConfig AppConfiguration;
	RequestManager reqMan;

	void _waitRequest();

public:
	// definire il construttore dopo perchè bisonga tenere conto dell'indirizzo del server e della porta
	Server();
	Server(const std::string ServerAddr, const int ListenPort);
	void closeServer();
};

#endif // !SERVER_H
