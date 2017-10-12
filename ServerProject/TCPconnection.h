#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <boost\asio.hpp>
#include <set>
#include <memory>
#include "Request.h"

#define BUFF_SIZE 1024

using namespace boost::asio;

/* 
	the class session is the super class of the TCP connection class 
*/
class Session {
protected:
	ip::tcp::endpoint clientAddr;
	Request clientRequest;

public:
	Session();
	void readRequest();
	void readDataChunks();
};

class TCPconnection: public Session {

/*
	the class TCPconnection contain a pointer  to a socekt, 
	2 buffers, 1 to read  and one to write
	2 mutable buffer, used in the boost library 
*/
private:
	char *read_data;
	char *write_data;
	mutable_buffer read_buff;
	mutable_buffer write_buff;
	std::shared_ptr<ip::tcp::socket> tcp_conn_socket;
	boost::system::error_code err;

	// isn't possible to copy an TCP connection instance
	TCPconnection(const TCPconnection& TCPconn) {}
	TCPconnection& operator=(const TCPconnection& TCPconn) {}

public:
	TCPconnection();
	~TCPconnection();
	
	void createConnection(io_service& io_serv);
	void closeConnection();
	ip::tcp::socket& getSocket();
};

/*
  this class contain all the connection instantiated
*/
class ConnectionPool {
private:
	std::set<std::shared_ptr<TCPconnection>> connSet;
public:
	ConnectionPool() = default;
	void start(std::shared_ptr<TCPconnection> conn);
	void stop(std::shared_ptr<TCPconnection> conn);
	void stop_all();

};

#endif	//TCP_CONNECTION_H

