#include "pch.h"
#include <chrono>
#include <thread>
#include <random>
#include <utility>


void Discovery::find_users() {
	udp_client_.send_broadcast(DISCOVERY_MSG);
}

BOOL Discovery::stop_discovery_service() {

	try {
		udp_server_.stop();

		if (discovery_thread_.joinable())
			discovery_thread_.join();

		return true;
	} catch(system_error& e) {

		if(e.code() == errc::no_such_process) {
			return true;
		}
		return false;
	}
}

void Discovery::start_udp_server() {

	int retry = 0;

	if (retry < 2) {
		try {
			start_listening();
		} catch (udp_exception::UdpShutdownException &e) {
			retry += 1;
		}
	}

	managed_callback::getInstance().call_exit();
}


void Discovery::set_username(std::string username) {
	hello_message_.set_username(username);
}

void Discovery::set_user_image(std::string image_name) {
	image_message.set_image(image_name);
}

void Discovery::start_listening() {

	char buffer[MAXBUFL] = "";
	//char username[MAXBUFL] = "";
	DiscoveryMessage packet;
	//struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	const auto client_address_ptr = &client_address;
	ZeroMemory(&client_address, sizeof(client_address));

	while (true) {

		socklen_t address_len;
		
		address_len = udp_server_.receive_datagram(buffer, client_address_ptr, MAXBUFL);

        packet.append(buffer, strlen(buffer));

		if (packet.get_packet_type() == DISCOVERY_MSG) {

			if (!hide_me_.load()) {
				cout << "HERE THE DISCOVERY RECEIVED: " << packet.get_message_body() << endl;

				//Wait random time before sending the hello message. So that the hosts won't be flooded
				std::mt19937_64 eng{ std::random_device{}() };  // or seed however you want
				const std::uniform_int_distribution<> distance{ 50, 200 };
				std::this_thread::sleep_for(std::chrono::milliseconds{ distance(eng) });

				udp_server_.send_datagram(hello_message_.get_message_body().c_str(), &client_address, address_len, strlen(hello_message_.get_message_body().c_str()));
				udp_server_.send_datagram(image_message.get_message_body().c_str(), &client_address, address_len, strlen(image_message.get_message_body().c_str()));

			}
		}
		else if (packet.get_packet_type() == HELLO_MSG) {

			cout << "HERE THE HELLO RECEIVED: " << packet.get_message_body() << "The username obviously is: " << packet.get_username() << endl;

			managed_callback::getInstance().call_discovery(udp_service::get_client_address(client_address_ptr).c_str(), packet.get_username().c_str(), "");

		} else if(packet.get_packet_type() == DISCOVERY_IMAGE) {
			
			auto image_name = packet.get_image_name();

			managed_callback::getInstance().call_discovery(udp_service::get_client_address(client_address_ptr).c_str(), "", image_name.c_str());

		}
		else
			cout << "IT WASN'T A DISCOVERY MESSAGE!" << endl;

		packet.clear();

		//send_datagram(buffer_, client_address_ptr, address_len, strlen(buffer_));
	}
}

void Discovery::start_discovery_service(std::string username, std::string image_name) {
	if(username.size() > 32)
		username.resize(32);
	udp_server_.start_server();
	hello_message_.set_username(std::move(username));
	image_message.set_image(image_name);
	discovery_thread_ = std::thread(&Discovery::start_udp_server, this);
	cout << "-------------- Server started" << endl;
}

Discovery::~Discovery(){
	stop_discovery_service();
}
