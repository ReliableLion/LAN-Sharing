#include "TCPconnection.h"

TCPconnection::TCPconnection() {


}


void TCPconnection::createConnection(io_service& io_serv) {

	// declaration of the socket
	tcp_conn_socket = std::shared_ptr<ip::tcp::socket>(new ip::tcp::socket(io_serv));

	if (!tcp_conn_socket->is_open()) {
		// the socket is close launch an exception
		throw std::exception();
	}

	// declaration of the buffer
	write_data = new char[BUFF_SIZE];
	read_data = new char[BUFF_SIZE];

	// incaplusate the data into the mutable buffer
	write_buff = mutable_buffer(write_data, BUFF_SIZE);
	read_buff = mutable_buffer(read_data, BUFF_SIZE);
}

void TCPconnection::closeConnection() {

	tcp_conn_socket->close(err);

	if (err) {
		// is not possible to close the socket throw an exception
	}
	else {
		// delete all the buffer allocated
		delete[] write_data;
		delete[] read_data;
	}

}

// return the connected socket
ip::tcp::socket& TCPconnection::getSocket() {
	return *tcp_conn_socket;
}

TCPconnection::~TCPconnection() {
	closeConnection();
}


void ConnectionPool::start(std::shared_ptr<TCPconnection> conn) {
	connSet.insert(conn);
}

void ConnectionPool::stop(std::shared_ptr<TCPconnection> conn) {
	// stop the connectiona and delete it from the set
	conn->closeConnection();
	connSet.erase(conn);
}

void ConnectionPool::stop_all() {

	// close all the connection in the set
	for (auto i = std::begin(connSet); i != std::end(connSet); i++) {
		std::shared_ptr<TCPconnection> conn = *i;
		conn->closeConnection();
	}

	// delete all the element in the set
	connSet.clear();

}
