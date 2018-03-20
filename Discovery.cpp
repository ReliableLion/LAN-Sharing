#include "Discovery.hpp"

void discovery::find_users() {

	udp_client_.send_broadcast(DISCOVERY_MSG);
}

void discovery::send_hello(){
	udp_client_.send_broadcast(hello_message_.get_message_body().c_str());
}

void discovery::start_discovery_service() {

	char buffer[MAXBUFL] = "";
	char username[MAXBUFL] = "";
	discovery_message packet;
	struct sockaddr_in server_address, client_address;

	const auto client_address_ptr = &client_address;
	ZeroMemory(&client_address, sizeof(client_address));

	while (true) {

		const auto address_len = udp_server_.receive_datagram(buffer, client_address_ptr, MAXBUFL);

		packet.Append(buffer, strlen(buffer));

		if (packet.get_packet_type() == DISCOVERY_MSG) {
			cout << "HERE THE DISCOVERY RECEIVED: " << buffer << endl;
			udp_server_.send_datagram(hello_message_.get_message_body().c_str(), client_address_ptr, sizeof(*client_address_ptr), strlen(hello_message_.get_message_body().c_str()));
		}
		else if (packet.get_packet_type() == HELLO_MSG) {
			cout << "HERE THE HELLO RECEIVED: " << buffer << endl;
			if (online_users_.find(udp_service::get_client_address(client_address_ptr))->second == packet.get_username(username))
				continue;
			else {
				online_users_.insert(udp_service::get_client_address(client_address_ptr), packet.get_username(username));
			}
		}
		else
			cout << "IT WASN'T A DISCOVERY MESSAGE!" << endl;

		packet.clear();

		//send_datagram(buffer_, client_address_ptr, address_len, strlen(buffer_));
	}
}
