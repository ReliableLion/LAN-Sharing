#pragma once
/*
THIS FILE IS CREATED USING AS EXAMPLE:
connection:
http://www.boost.org/doc/libs/1_65_1/doc/html/boost_asio/example/cpp03/http/server3/connection.cpp
timeout:
http://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/example/timeouts/server.cpp
*/

#include <boost\asio.hpp>
#include <boost\array.hpp>
#include <boost\asio\deadline_timer.hpp>
#include <set>
#include <memory>
#include <iostream>

#include "Message.hpp"

// 1526 Bytes (each frame) - 26 B (Ethernet header) - 20 B (IP header) - 20 B (TCP header) = 1460 Bytes of data
#define CHUNK_SIZE 1460

using namespace boost::asio;
using boost::asio::ip::tcp;

/*
this is the parent class of TCPconnection. It's used as abstraction of multiple transactions between the server
and a remote end-point using  one or more TCP connections.
It defines a timeout mechanism usefull to close the connection if is no longer used.
https://devcentral.f5.com/questions/connections-vs-sessions
Connection vs Session: multiple transaction between the client and the server are considered a session.
*/


/*
the class TCPConnection contain a socket,
2 buffers, 1 to read  and one to write
2 mutable buffer, used in the boost library
This class models a connection that make use of the TCP protocol, inside there are the buffers for the I/O,
a connected socket and various instance methods. This functions allows read and write operations on the socket
and to modify the connection status
*/
class TCPConnection {
protected:

	//boost::array<void, CHUNK_SIZE> chunk;

	ip::tcp::endpoint clientAddr;
	deadline_timer input_deadline;
	deadline_timer output_deadline;

	ip::tcp::socket sock;
	boost::system::error_code err;

public:
	TCPConnection(io_service& io_serv);
	~TCPConnection();
	void closeConnection();
	bool checkConnection();
	bool writeRequestMessage(RequestMessage msg);
	RequestMessage readRequestMessage();

	bool writeDiscoveryMessage(DiscoveryMessage msg);
	DiscoveryMessage readDiscoveryMessage();

	ip::tcp::socket& getSocket();
	boost::system::error_code getError();
};

/*
connection pool class save all the connection that the server receives
*/
class ConnectionPool {
private:
	std::set<std::shared_ptr<TCPConnection>> connSet;
public:
	ConnectionPool() = default;
	void start(std::shared_ptr<TCPConnection> conn);
	void stop(std::shared_ptr<TCPConnection> conn);
	void stop_all();
};

class TCPClientConnection : public TCPConnection {

private:
	tcp::resolver resolver;

public:
	TCPClientConnection(io_service& io_serv);
	void connect(std::string serverAddress, std::string serverPort);

};
