#pragma once

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

enum { max_length = 1024 };
const int udpPort = 1234;

/*
class UDPServer {

public:
	UDPServer(boost::asio::io_service& io_service, string resp)
		: io_service_(io_service),
		socket_(io_service, udp::endpoint(udp::v4(), udpPort)) {
		response = resp;
		socket_.async_receive_from(
			boost::asio::buffer(data_, max_length), sender_endpoint_,
			boost::bind(&UDPServer::handle_receive_from, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void setResponse(string resp) {
		response = resp;
	}

	void startServer() {
		io_service_.run();
	}

	void handle_receive_from(const boost::system::error_code& error,
		size_t bytes_recvd) {

		if (!error && bytes_recvd > 0) {

			if (strcmp(data_, "LAN-SHARING LOOKINGFOR\r\n") == 0) {

				socket_.async_send_to(
					boost::asio::buffer(response), sender_endpoint_,
					boost::bind(&UDPServer::handle_send_to, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
			}
		}
		else {

			socket_.async_receive_from(
				boost::asio::buffer(data_, max_length), sender_endpoint_,
				boost::bind(&UDPServer::handle_receive_from, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
	}

	void handle_send_to(const boost::system::error_code& ,
		size_t) {

		socket_.async_receive_from(
			boost::asio::buffer(data_, max_length), sender_endpoint_,
			boost::bind(&UDPServer::handle_receive_from, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

private:
	boost::asio::io_service& io_service_;
	udp::socket socket_;
	udp::endpoint sender_endpoint_;
	char data_[max_length];

	string response;
};

class UDPClient {

public:
	UDPClient(): 
		sock(io_service, udp::endpoint(udp::v4(), 0)) {
		reply[0] = '\0';
	}

	void sendMessage(std::string host, std::string port, std::string request) {

		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), host, port);
		udp::resolver::iterator iterator = resolver.resolve(query);

		using namespace std; // For strlen.

		size_t request_length = request.length();
		sock.send_to(boost::asio::buffer(request, request_length), *iterator);
	}

	void sendBroadcastMessage(std::string request) {

		using namespace std; // For strlen.

		sock.set_option(boost::asio::socket_base::broadcast(true));

		boost::asio::ip::udp::endpoint broadcastEndpoint(boost::asio::ip::address_v4::broadcast(), udpPort);

		size_t request_length = request.length();
		sock.send_to(boost::asio::buffer(request, request_length), broadcastEndpoint);
	}

	std::pair<std::string, std::string> receiveMessage() {

		udp::endpoint sender_endpoint;
		cout << "Receiveing" << endl;

		size_t reply_length = sock.receive_from(
			boost::asio::buffer(reply, max_length), sender_endpoint);
		cout << "Received" << endl;

		string response(reply, reply_length);

		if (reply_length == 0)
			return make_pair("","");
		else
			return make_pair(response, sender_endpoint.address().to_string());
	}

private:
	boost::asio::io_service io_service;
	char reply[max_length];

	udp::socket sock;

}; */