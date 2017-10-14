#include "TCPconnection.h"

TCPconnection::TCPconnection(io_service& io_serv) : io(io_serv), TCP_session(io_serv) {}

void TCPconnection::createConnection() {

	// declaration of the socket
	tcp_conn_socket = std::shared_ptr<ip::tcp::socket>(new ip::tcp::socket(io));

	if (!tcp_conn_socket->is_open()) {
		// the socket is close launch an exception
		throw std::exception();
	}
	/*
	// declaration of the buffer
	write_data = new char[BUFF_SIZE];
	read_data = new char[BUFF_SIZE];

	// incaplusate the data into the mutable buffer
	write_buff = mutable_buffer(write_data, BUFF_SIZE);
	read_buff = mutable_buffer(read_data, BUFF_SIZE);
	*/

	// save the remote endpoint
	TCP_session::clientAddr = tcp_conn_socket->remote_endpoint();
}

void TCPconnection::closeConnection() {

	// check if the connection is already close
	if (!tcp_conn_socket->is_open()) {
		//throw a new exception
	}

	tcp_conn_socket->close(err);

	if (err) {
		// is not possible to close the socket throw an exception
	}
	else {
		/*
		delete[] write_data;
		delete[] read_data;
		*/
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

TCP_session::TCP_session(io_service& io) : input_deadline(io), output_deadline(io) {}

// this method can read an upcoming request
void TCPconnection::readRequest() {
	// setup a wait timer 
	input_deadline.expires_from_now(boost::posix_time::seconds(30));

	// call the methos inseide the readRequest 
	clientRequest.readRequest();

	input_deadline.async_wait(&TCPconnection::_check_deadline);
}

void TCPconnection::readDataChunks() {}

void TCPconnection::writeReply() {}

void TCPconnection::_check_deadline(deadline_timer timer) {

	// check if the socket si connected or not
	if (!tcp_conn_socket->is_open()) {
		//launch an exception
	}

	// checke if the connection reached the deadline, then close the connection 
	if (timer.expires_at() <= deadline_timer::traits_type::now()) {
		closeConnection();
	}

}