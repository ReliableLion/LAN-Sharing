#include "TCPConnection.hpp"

//---------------TCPconnecton Class

// in the constructor is declared the socket element 
TCPConnection::TCPConnection(io_service& io_serv) : sock(io_serv), input_deadline(io_serv), output_deadline(io_serv) {

	if (!sock.is_open()) {
		// the socket is close launch an exception
		throw std::exception();
	}

	input_deadline.expires_at(boost::posix_time::pos_infin);
	output_deadline.expires_at(boost::posix_time::pos_infin);

}

TCPConnection::~TCPConnection() { closeConnection(); }

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

void TCPConnection::closeConnection() {

	// check if the connection is already close
	if (!sock.is_open()) {
		//throw a new exception
		std::cout << "socket closed already closed" << std::endl;
		return;
	}

	sock.close(err);

	if (err) {
		throw std::exception();
	}

}

// return the connected socket
ip::tcp::socket& TCPConnection::getSocket() {
	return sock;
}

bool TCPConnection::checkConnection() {
	return sock.is_open();
}

boost::system::error_code TCPConnection::getError() { return err; }

//-------------Connection Pool Class


// insert a new connection in the set of connection and wait for the client request
void ConnectionPool::start(std::shared_ptr<TCPConnection> conn) {
	connSet.insert(conn);
}

void ConnectionPool::stop(std::shared_ptr<TCPConnection> conn) {
	// stop the connectiona and delete it from the set
	conn->closeConnection();
	connSet.erase(conn);
}

void ConnectionPool::stop_all() {

	// close all the connection in the set
	for (auto i = std::begin(connSet); i != std::end(connSet); i++) {
		std::shared_ptr<TCPConnection> conn = *i;
		conn->closeConnection();
	}

	// delete all the element in the set
	connSet.clear();
}

bool TCPConnection::writeMessage(Message msg) {

	size_t byteTransferred = write(this->sock, msg.getMessageData());

	if (byteTransferred != msg.getMessageSize())
		return false;

	return true;
}

RequestMessage TCPConnection::readRequestMessage() {

	RequestMessage message;
	std::vector<const_buffer> bufferContainer;
	boost::system::error_code errorCode;

	size_t byteRead = read(sock, message.requestBuffer, errorCode);

	if (!errorCode)
		return message;
	else
		throw std::exception();

}

TCPClientConnection::TCPClientConnection(io_service& io_serv) : TCPConnection(io_serv), resolver(io_serv) {}

void TCPClientConnection::connect(std::string serverAddress, std::string serverPort) {

	tcp::resolver::query query(tcp::v4(), serverAddress, serverPort);
	tcp::resolver::iterator iterator = resolver.resolve(query);

	boost::asio::connect(sock, iterator);
}