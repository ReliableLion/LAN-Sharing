#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <memory>
#include "TCPconnection.h"

/*
	preliminar ack to setup all the donwload paramters
*/
class Request {
private:
	std::string msg;

public:
	Request();
	void readRequest();
};

#endif // !CLIENT_ACK_H

