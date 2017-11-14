#ifndef SERVER_H
#define SERVER_H 

#include <iostream>
#include <atomic>
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
	
	std::atomic<bool> is_passive;

	void _waitRequest();
	bool _portChecking(int port_number);
	void _acceptTCPConnection(std::shared_ptr<TCPconnection_server> connection, boost::system::error_code err, std::time_t timestamp);
	void _refuseTCPConnection(std::shared_ptr<TCPconnection_server> connection, boost::system::error_code err);

public:
	// definire il construttore dopo perchè bisonga tenere conto dell'indirizzo del server e della porta
	Server();
	Server(const std::string ServerAddr, const int ListenPort);
	~Server();
	void startServer();
	void pauseServer();
	void restartServer();
	void closeServer();
};

#endif // !SERVER_H
