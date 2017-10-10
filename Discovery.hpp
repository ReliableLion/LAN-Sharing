#pragma once

#include "UDPService.hpp"

using namespace std;

class Discovery {

public:
	Discovery(string username){
		Discovery::username = username;
	}

	// Check if a user is still online
	bool isUserOnline(string userIp);

	// Return a list of pairs (UserIP, Username)
	map<string, string> findUsers();

	void sendHello(string host, string port);

private:
	string username;
};