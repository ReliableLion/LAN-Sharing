#pragma once
#include "UDPService.hpp"
#include "Message.hpp"

using namespace std;

class Discovery {

public:
	Discovery(string username){
		Discovery::username = username;
	}

	void startDiscoveryService();

	// Check if a user is still online
	bool isUserOnline(string userIp);

	// Return a list of pairs (UserIP, Username)
	std::map<std::string, std::string> findUsers();
	std::map<std::string, std::string> findUser(string host, string port);

private:
	string username;
};