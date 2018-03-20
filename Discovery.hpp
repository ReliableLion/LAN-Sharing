#pragma once
#include "UDPService.hpp"
#include "Message.hpp"

using namespace std;

class discovery {

public:
	explicit discovery(const string username): hello_message_(this->my_username_) {
		my_username_ = username;
	}

	void start_discovery_service();

	void send_hello();
	// Return a list of pairs (UserIP, Username)
	void find_users();

private:
	string my_username_;
	discovery_message hello_message_;
	map<string, string> online_users_;

	udp_service::udp_server udp_server_;
	udp_service::udp_client udp_client_;
};