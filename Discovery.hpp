#pragma once
#include "UDPService.hpp"
#include "Message.hpp"

using namespace std;

class discovery {

public:
	explicit discovery(const string username){
		my_username_ = username;
	}

	void start_discovery_service();

	void send_hello();
	// Return a list of pairs (UserIP, Username)
	void find_users();
	// Check if a user is still online
	std::map<string, std::string> find_user(std::string username);

private:
	string my_username_;
	map<string, string> online_users_;

	udp_service::udp_server udp_server_;
	udp_service::udp_client udp_client_;
};