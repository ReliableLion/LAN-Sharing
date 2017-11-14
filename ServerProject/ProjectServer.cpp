#include "ProjectServer.h"

/*
	create a new server instance and resolve the endpoint name 
*/
Server::Server(std::string ServerAddr, int  ListenPort) : io(), Acceptor(io), 
		d_man_ptr(std::shared_ptr<DownloadManager> (new DownloadManager())), reqMan(d_man_ptr) {

	this->ServerAddr = ServerAddr;
	this->int_ServerPort = ListenPort;
	this->ServerPort = std::to_string(ListenPort);

	if (_portChecking(int_ServerPort)) {
		std::cout << "the port specified is not correct" << std::endl;
		throw std::exception();
	}

	ip::tcp::resolver resolver(io);
	ip::tcp::resolver::query query(this->ServerAddr, this->ServerPort);
	local_endpoint = *resolver.resolve(query);
	d_man_ptr = std::shared_ptr<DownloadManager>(new DownloadManager());
}

/*
	create a new server instance using default configurations
*/
Server::Server() : io(), Acceptor(io), 
		d_man_ptr(std::shared_ptr<DownloadManager> (new DownloadManager())), reqMan(d_man_ptr) {
	
	ip::tcp::resolver resolver(io);
	local_endpoint = ip::tcp::endpoint(ip::tcp::v4(), int_ServerPort);
	d_man_ptr = std::shared_ptr<DownloadManager>(new DownloadManager());
}

Server::~Server() {
	Acceptor.close();
	reqMan.shutdown();
	reqMan.closeConnections();
	d_man_ptr->exitDownloader();
}

void Server::startServer() {
	Acceptor.open(local_endpoint.protocol());
	Acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
	Acceptor.bind(local_endpoint);
	Acceptor.listen();

	// set server active
	is_passive.store(false);

	// begin to wait for request
	_waitRequest();
}

void Server::pauseServer() {
	if (is_passive.load()) {
		// lauch a new exception 
	}
	else {
		is_passive.store(true);
	}
}

void Server::restartServer() {
	is_passive.store(false);
}

void Server::closeServer() {
	// close all possible instances declarated into the server
	Acceptor.close();
	reqMan.shutdown();
	reqMan.closeConnections();
	d_man_ptr->exitDownloader();
}

// this function wait an inbound request from the applcation client
void Server::_waitRequest() {
	std::time_t timestamp;
	boost::system::error_code err;
	
	std::cout << "server initialization" << std::endl << std::endl;

	// iterate if the configuration of the app is setted to PUBLIC
	// TODO remember to change the true in order to stop the server 
	while (true) {
		//? is better to instantiate a new connection before or after the accept?
		std::shared_ptr<TCPconnection_server> newConnection = std::shared_ptr<TCPconnection_server>(new TCPconnection_server(io));

		std::cout << "Server on IP address: " << local_endpoint.address() << ", port: " << local_endpoint.port() << " wait for incoming request..." << std::endl;
		Acceptor.accept(newConnection->getSocket(), err);
		timestamp = std::time(nullptr);

		// TODO use a mechanism to refuse the connection if the server is in passive mode
		if (is_passive.load() == false) {
			_acceptTCPConnection(newConnection, err, timestamp);
		}
		else {
			_refuseTCPConnection(newConnection, err);
		}
		
	}	
}

bool Server::_portChecking(int port_number) {
	// check if the port is betwenn 0 and 63555
	if (port_number > MAX_PORT_N || port_number < MIN_PORT_N) {
		return false;
	}
	else {
		return true;
	}
}

void Server::_acceptTCPConnection(std::shared_ptr<TCPconnection_server> connection, boost::system::error_code err, std::time_t timestamp) {
	if (err) {
		std::cout << "connection refused, error: " << err << std::endl << std::endl;
	}
	else {
		//? is the code below necessary for the application?
		ip::tcp::endpoint remote_endpoint = connection->getRemoteEndpoint();
		std::cout << "(" << std::asctime(std::localtime(&timestamp)) << ")" << " server accepted an incoming request:" << std::endl;
		std::cout << "address: " << remote_endpoint.address() << std::endl;
		std::cout << "port: " << remote_endpoint.port() << std::endl << std::endl;
		if (reqMan.addRequest(connection) == false) { std::cout << "is not possible to add a new connection in the queue" << std::endl; }
	}
}

void Server::_refuseTCPConnection(std::shared_ptr<TCPconnection_server> connection, boost::system::error_code err) {
	if (err) {
		std::cout << "connection refused, error: " << err << std::endl << std::endl;
	}
	else {
		// send a error msg error and close the connection

		//TODO send a msg to say that the server not accept incoming connection

		connection->closeConnection();
	}
}