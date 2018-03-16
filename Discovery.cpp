#include "Discovery.hpp"

std::map<string, std::string> Discovery::find_users() {

	udp_service::udp_client udp_client;

	auto hello_message = discovery_message(this->username_);

	udp_client.send_broadcast();

	this->online_users_ = udp_client.get_online_users();
	return this->online_users_;
}

std::map<string, std::string> Discovery::find_user(std::string username) {

	udp_service::udp_client udp_client;

	auto hello_message = discovery_message(this->username_);
	this->online_users_.find(username.c_str());
	udp_client.get_server_info(this->online_users_.find(username)->second, std::to_string(UDP_PORT));
	udp_client.send_datagram(hello_message.get_message_body());

	auto user = udp_client.get_online_users();

	if(user.size() > 1) {
		if (user.find(username)->first == username)
			return user;
	} else if (user.size() == 1) {
		if (user.find(username)->first == username)
			return user;
	}

	user.clear();
	return user;
}

void Discovery::start_discovery_service() {

	discovery_message helloMessage = discovery_message(this->username_);

	//UDPServer udpServer(io_service, helloMessage.getMessageBody());

	//udpServer.startServer();
}