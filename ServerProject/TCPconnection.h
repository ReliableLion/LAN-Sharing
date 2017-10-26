/*
	THIS FILE IS CREATED USING AS EXAMPLE: 

	connection:
	http://www.boost.org/doc/libs/1_65_1/doc/html/boost_asio/example/cpp03/http/server3/connection.cpp

	timeout:
	http://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/example/timeouts/server.cpp

*/


#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <boost\asio.hpp>
#include <boost\asio\ssl.hpp>
#include <boost\asio\deadline_timer.hpp>
#include <queue>
#include <memory>
#include <iostream>

#include "Message.hpp"
#include "Buffer.h"

using namespace boost::asio;

/* 
	this is the parent class of TCPconnection. Is used as abstraction of multiple transaction between the server
	and a remote end-point using  one or more TCP connections.
	It defines a timeout mechanism usefull to close the connection if is no longer used. 

	https://devcentral.f5.com/questions/connections-vs-sessions
	Connection vs Session: multiple transaction between the client and the server are considered a session.
*/


/*
	the class TCPconnection contain a socekt,
	2 buffers, 1 to read  and one to write
	2 mutable buffer, used in the boost library

	This class model a connection that make use of the TCP protcol, inside there are the buffers for the I/O, 
	a connected socket and various instance method. This functions allows read and write operations on the socket
	and to modify the connection status

*/
class TCPconnection {
protected:
	
	/*boost::array<void, BUFF_SIZE> write_buff;
	boost::array<void, BUFF_SIZE> read_buff;*/

	streambuf readbuff;
	streambuf writebuff;

	ip::tcp::endpoint clientAddr;
	deadline_timer input_deadline;
	deadline_timer output_deadline;

	ip::tcp::socket sock;
	boost::system::error_code err;

public:
	TCPconnection(io_service& io_serv);
	~TCPconnection();
	void closeConnection();
	bool checkConnection();
	ip::tcp::socket& getSocket();
	boost::system::error_code getError();
	ip::tcp::endpoint getRemoteEndpoint();
};


class TCPconnection_server : public TCPconnection {

public:
	TCPconnection_server(io_service& io): TCPconnection(io) {}
	~TCPconnection_server() {}
	size_t readDataChunks(std::shared_ptr<buffer_type> chunk_buffer);
	std::string readRequest(RequestMessage &msg);
	void writeReply(ReplyMsg &msg);
};

#endif	//TCP_CONNECTION_H