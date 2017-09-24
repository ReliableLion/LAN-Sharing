#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <boost\asio.hpp>
#define BUFF_SIZE 1024

using namespace boost::asio;

class TCPconnection {

private:
	mutable_buffer<void, BUFF_SIZE> read_buffer;
	mutable_buffer<void, BUFF_SIZE>	write_buffer;
	std::shared_ptr<ip::tcp::socket> tcp_conn_socket;

public:
	TCPConnection();
	void createConnction();
	void closeConnection();
};

#endif	//TCP_CONNECTION_H

