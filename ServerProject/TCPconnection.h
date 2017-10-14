#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <boost\asio.hpp>
#include <boost\array.hpp>
#include <boost\asio\deadline_timer.hpp>
#include <set>
#include <memory>
#include "Request.h"

#define BUFF_SIZE 1024

using namespace boost::asio;


/* 
	the class session is the super class of the TCP connection class 
	Manange the TCP connection, implemnt a  mechanism to wait until a deadline is reached
*/
class TCP_session {
protected:
	ip::tcp::endpoint clientAddr;
	Request clientRequest;
	deadline_timer input_deadline;
	deadline_timer output_deadline;

public:
	TCP_session(io_service& io);
	virtual ~TCP_session();
	virtual void readRequest() = 0;
	virtual void readDataChunks() = 0;
	virtual void writeReply() = 0;
};


/*
the class TCPconnection contain a pointer  to a socekt,
2 buffers, 1 to read  and one to write
2 mutable buffer, used in the boost library
*/
class TCPconnection: public TCP_session {
private:
	char *read_data;
	char *write_data;
	
	/*
	mutable_buffer read_buff;
	mutable_buffer write_buff;
	*/
	boost::array<void, BUFF_SIZE> write_buff;
	boost::array<void, BUFF_SIZE> read_buff;

	io_service io;
	std::shared_ptr<ip::tcp::socket> tcp_conn_socket;
	boost::system::error_code err;

	// isn't possible to copy an TCP connection instance
	TCPconnection(const TCPconnection& TCPconn) {}
	TCPconnection& operator=(const TCPconnection& TCPconn) {}

public:
	TCPconnection(io_service& io_serv);
	~TCPconnection();
	void createConnection();
	void closeConnection();
	ip::tcp::socket& getSocket();
	
	// inherted methods 
	void readRequest();
	void readDataChunks();
	void writeReply();
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

