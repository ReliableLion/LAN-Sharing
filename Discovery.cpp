#include "Discovery.hpp"
#include "Message.hpp"
#include "UDPService.hpp"

std::map<std::string, std::string> Discovery::findUsers() {

	UDPClient udpClient;
	map<std::string, std::string> users;
	pair<std::string, std::string> userPair;

	DiscoveryMessage helloMessage = DiscoveryMessage(this->username);

	udpClient.sendBroadcastMessage(helloMessage.getMessageBody());

	while (!((userPair = udpClient.receiveMessage()).first.empty()))
		users.insert(userPair);
	// TO DO: ADD TIMER

	return users;
}

std::map<std::string, std::string> Discovery::findUser(string host, string port) {

	UDPClient udpClient;
	map<std::string, std::string> users;
	pair<std::string, std::string> userPair;

	DiscoveryMessage helloMessage = DiscoveryMessage(this->username);

	udpClient.sendMessage(host, port, helloMessage.getMessageBody());

	userPair = udpClient.receiveMessage();
	users.insert(userPair);

	return users;
}

void Discovery::startDiscoveryService() {

	boost::asio::io_service io_service;

	DiscoveryMessage helloMessage = DiscoveryMessage(this->username);

	UDPServer udpServer(io_service, helloMessage.getMessageBody());

	udpServer.startServer();
}