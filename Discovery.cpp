#include "Discovery.hpp"
#include <chrono>
#include <thread>
#include <random>
#include <iostream>

void Discovery::find_users() {
	udp_client_.send_broadcast(DISCOVERY_MSG);
}

map<string, string> Discovery::get_online_users() const {
	return online_users_;
}

/*
void discovery::send_hello(){
	udp_client_.send_broadcast(hello_message_.get_message_body().c_str());
}*/

void Discovery::start_listening() {

	udp_service::UdpServer udp_server;

	char buffer[MAXBUFL] = "";
	//char username[MAXBUFL] = "";
	DiscoveryMessage packet;
	//struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	const auto client_address_ptr = &client_address;
	ZeroMemory(&client_address, sizeof(client_address));

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
	while (true) {

		const auto address_len = udp_server.receive_datagram(buffer, client_address_ptr, MAXBUFL);

        packet.append(buffer, strlen(buffer));

		if (packet.get_packet_type() == DISCOVERY_MSG) {
			cout << "HERE THE DISCOVERY RECEIVED: " << packet.get_message_body() << endl;
		
			//Wait random time before sending the hello message. So that the hosts won't be flooded
			std::mt19937_64 eng{ std::random_device{}() };  // or seed however you want
			const std::uniform_int_distribution<> distance{ 50, 200 };
			std::this_thread::sleep_for(std::chrono::milliseconds{ distance(eng) });

			udp_server.send_datagram(hello_message_.get_message_body().c_str(), &client_address, address_len, strlen(hello_message_.get_message_body().c_str()));
		}
		else if (packet.get_packet_type() == HELLO_MSG) {
			cout << "HERE THE HELLO RECEIVED: " << packet.get_message_body() << "The username obviously is: " << packet.get_username() << endl;
			
			if (online_users_.size() > 0) {
				online_users_.insert_or_assign(udp_service::get_client_address(client_address_ptr), packet.get_username());

			} else
				online_users_.insert(make_pair(udp_service::get_client_address(client_address_ptr), packet.get_username()));
		}
		else
			cout << "IT WASN'T A DISCOVERY MESSAGE!" << endl;

		packet.clear();

		//send_datagram(buffer_, client_address_ptr, address_len, strlen(buffer_));
	}
#pragma clang diagnostic pop
}

void Discovery::start_discovery_service() {
	server_thread_ = std::async(&Discovery::start_listening, this);

	cout << "-------------- Server started" << endl;
}

