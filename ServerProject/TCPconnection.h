/*
	THIS FILE IS CREATED USING AS EXAMPLE: 

	connection:
	http://www.boost.org/doc/libs/1_65_1/doc/html/boost_asio/example/cpp03/http/server3/connection.cpp

	timeout:


*/


#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <boost\asio.hpp>
#include <boost\array.hpp>
#include <boost\asio\deadline_timer.hpp>
#include <set>
#include <memory>

#define BUFF_SIZE 1024

using namespace boost::asio;

/* 
	this is the parent class of TCPconnection. Is used as abstraction of multiple transaction between the server
	and a remote end-point using  one or more TCP connections.
	It defines a timeout mechanism usefull to close the connection if is no longer used. 

	https://devcentral.f5.com/questions/connections-vs-sessions
	Connection vs Session: multiple transaction between the client and the server are considered a session.
*/
class TCP_session {
protected:
	ip::tcp::endpoint clientAddr;
	deadline_timer input_deadline;
	deadline_timer output_deadline;

	void _check_deadline(deadline_timer timer);

public:
	TCP_session(io_service& io);

};


/*
	the class TCPconnection contain a socekt,
	2 buffers, 1 to read  and one to write
	2 mutable buffer, used in the boost library

	This class model a connection that make use of the TCP protcol, inside there are the buffers for the I/O, 
	a connected socket and various instance method. This functions allows read and write operations on the socket
	and to modify the connection status

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

	ip::tcp::socket sock;
	std::shared_ptr<ip::tcp::socket> tcp_conn_socket;
	boost::system::error_code err;

	/* isn't possible to copy an TCP connection instance
	TCPconnection(const TCPconnection& TCPconn) {}
	TCPconnection& operator=(const TCPconnection& TCPconn) {}*/

	void _check_deadline(deadline_timer timer);

public:
	TCPconnection(io_service& io_serv);
	~TCPconnection();
	void createConnection();
	void closeConnection();
	ip::tcp::socket& getSocket();
	

};

/*
  connection pool class save all the connection that the server receives
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

