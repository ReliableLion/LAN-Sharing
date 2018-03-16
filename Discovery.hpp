#pragma once
#include "UDPService.hpp"
#include "Message.hpp"

using namespace std;

class Discovery {

public:
	Discovery(string username){
		Discovery::username_ = username;
	}

	void start_discovery_service();

	// Check if a user is still online
	bool is_user_online(string user_ip);

	// Return a list of pairs (UserIP, Username)
	std::map<string, std::string> find_users();
	std::map<string, std::string> find_user(std::string username);

private:
	string username_;
	map<string, string> online_users_;
};