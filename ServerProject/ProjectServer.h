#pragma once

#ifndef SERVER_H
#define SERVER_H 

#include <thread>
#include "ProjectServer.h"
#include "HandleDownload.h"
#include "UserConfiguration.h"


#include <boost/asio.hpp>

using namespace boost::asio;

/*
	this class should wait for the incoming request from the client 
*/
class Server {
private:
	io_service io;
	const int ServerPort = 1500;
	UserConfig AppConfiguration;

public:
	// definire il construttore dopo perchè bisonga tenere conto dell'indirizzo del server e della porta
	Server() {}
	void WaitRequest();

};



#endif // !SERVER_H
