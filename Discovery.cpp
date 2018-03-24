#include "Discovery.hpp"
#include <chrono>
#include <thread>
#include <random>

void discovery::find_users() {
	udp_client_.send_broadcast(DISCOVERY_MSG);
}

void discovery::send_hello(){
	udp_client_.send_broadcast(hello_message_.get_message_body().c_str());
}

void discovery::start_listening() {

	udp_service::udp_server udp_server_;

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
			cout << "HERE THE DISCOVERY RECEIVED: " << packet.get_message_body() << endl;
		
			std::mt19937_64 eng{ std::random_device{}() };  // or seed however you want
			const std::uniform_int_distribution<> distance{ 50, 200 };
			std::this_thread::sleep_for(std::chrono::milliseconds{ distance(eng) });

			udp_server_.send_datagram(hello_message_.get_message_body().c_str(), &client_address, address_len, strlen(hello_message_.get_message_body().c_str()));
		}
		else if (packet.get_packet_type() == HELLO_MSG) {
			cout << "HERE THE HELLO RECEIVED: " << packet.get_message_body() << "The username obviously is: " << packet.get_username() << endl;
			
			if (online_users_.size() > 0) {
				if (online_users_.find(udp_service::get_client_address(client_address_ptr))->second == packet.get_username())
					continue;
				
				online_users_.insert(make_pair(udp_service::get_client_address(client_address_ptr), packet.get_username()));

			} else
				online_users_.insert(make_pair(udp_service::get_client_address(client_address_ptr), packet.get_username()));
		}
		else
			cout << "IT WASN'T A DISCOVERY MESSAGE!" << endl;

		packet.clear();

		//send_datagram(buffer_, client_address_ptr, address_len, strlen(buffer_));
	}
}

void discovery::start_discovery_service() {
	server_thread_ = std::async(&discovery::start_listening, this);

	cout << "-------------- Server started" << endl;
}

