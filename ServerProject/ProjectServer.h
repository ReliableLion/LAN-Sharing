#ifndef SERVER_H
#define SERVER_H 

#include <iostream>
#include <thread>
#include <string>
#include <ctime>
#include "RequestMananger.h"
#include "UserConfiguration.h"

#include <boost/asio.hpp>

#define MAX_PORT_N 65535
#define MIN_PORT_N 0

using namespace boost::asio;

/*
	this class should wait for the incoming request from the client 
*/
class Server {
private:
	io_service io;
	ip::tcp::endpoint local_endpoint;
	std::string ServerPort = "1500";
	int int_ServerPort = 1500;
	std::string ServerAddr = "localhost";
	ip::tcp::acceptor Acceptor;

	UserConfiguration AppConfiguration;
	RequestManager reqMan;
	std::shared_ptr<DownloadManager> d_man_ptr;

	void _waitRequest();
	bool _portChecking(int port_number);

public:
	// definire il construttore dopo perchè bisonga tenere conto dell'indirizzo del server e della porta
	Server();
	Server(const std::string ServerAddr, const int ListenPort);
	~Server();
	void createServer();
	void closeServer();
};

#endif // !SERVER_H
