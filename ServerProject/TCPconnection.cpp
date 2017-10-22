#include "TCPconnection.h"

//---------------TCPconnecton Class

// in the constructor is declared the socket element 
TCPconnection::TCPconnection(io_service& io_serv) : sock(io_serv), input_deadline(io_serv), output_deadline(io_serv),
													readbuff(BUFF_SIZE), writebuff(BUFF_SIZE) {

	if (!sock.is_open()) {
		// the socket is close launch an exception
		throw std::exception();
	}

	input_deadline.expires_at(boost::posix_time::pos_infin);
	output_deadline.expires_at(boost::posix_time::pos_infin);

	// save the remote endpoint
	clientAddr = sock.remote_endpoint();
}

TCPconnection::~TCPconnection() { closeConnection(); }

/*void TCPconnection::createConnection() {

	// declaration of the socket
	tcp_conn_socket = std::make_shared<ip::tcp::socket>(sock);

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
	*

	// save the remote endpoint
	TCP_session::clientAddr = tcp_conn_socket->remote_endpoint();
}*/

void TCPconnection::closeConnection() {

	// check if the connection is already close
	if (!sock.is_open()) {
		//throw a new exception
		std::cout << "socket closed already closed" << std::endl;
		return;
	}

	sock.close(err);

	if (err) {
		// is not possible to close the socket throw an exception
	}
	else {

	}

}

// return the connected socket
ip::tcp::socket& TCPconnection::getSocket() {
	return sock;
}

bool TCPconnection::checkConnection() {
	return sock.is_open();
}

boost::system::error_code TCPconnection::getError() { return err; }

//-------------TCPconnection_server

// callback function called when the SO finish to read the data from the socket
void TCPconnection_server::_handle_read_until(boost::system::error_code& error) {
	
	// first of all check the timeout, in case is expired the timeout
	if (!checkConnection()) {
		return;
	}


}

// callbakc fucntion called whwn the SO finis to write the reply 
void TCPconnection_server::_handle_write(boost::system::error_code& error) {

	// check if the connection is open
	if (!checkConnection()) {
		return;
	}
	

}

void TCPconnection_server::readDataChunks() {}

// read the client request by using a read_until 
std::string TCPconnection_server::readRequest(RequestMessage& msg) {
	
	// requestMessage instance
	size_t read_byte;
	std::string request;

	if ((read_byte = read_until(sock, readbuff, msg.getEndMessage, err)) == 0) {
		//lauch a new exception 
	}

	if (!err) {
		readbuff.commit(read_byte);
		std::istream input_stream(&readbuff);

		// store the request message into a string
		input_stream >> request;
	}
	else {
		// if there is an error launch a new excpetion
	}

	return request;
}

void TCPconnection_server::writeReply() {
	
}