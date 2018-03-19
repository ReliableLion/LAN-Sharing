#include "Discovery.hpp"

void discovery::find_users() {

	udp_client_.send_broadcast(DISCOVERY_MSG);

	//this->online_users_ = udp_client_.get_online_users();

	//return this->online_users_;
}

void discovery::send_hello(){

	auto hello_message = discovery_message(this->my_username_);

	udp_client_.send_broadcast(hello_message.get_message_body().c_str());
}


std::map<string, std::string> discovery::find_user(std::string username) {

	udp_service::udp_client udp_client;

	auto hello_message = discovery_message(this->my_username_);
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

void discovery::start_discovery_service() {

	char buffer[MAXBUFL] = "";
	discovery_message packet;
	struct sockaddr_in server_address, client_address;

	const auto client_address_ptr = &client_address;
	ZeroMemory(&client_address, sizeof(client_address));

	//while (true) {

	const auto address_len = udp_server_.receive_datagram(buffer, client_address_ptr, MAXBUFL);

	packet.Append(buffer, strlen(buffer));

	if (packet.get_packet_type() == DISCOVERY_MSG)
		cout << "HERE THE DISCOVERY RECEIVED: " << buffer << endl;
	else if (packet.get_packet_type() == HELLO_MSG)
		cout << "HERE THE HELLO RECEIVED: " << buffer << endl;
	else
		cout << "IT WASN'T A DISCOVERY MESSAGE!" << endl;

	//send_datagram(buffer_, client_address_ptr, address_len, strlen(buffer_));
	//}
}