#pragma once
#include "UDPService.hpp"
#include "Message.hpp"
#include <future>

using namespace std;

class Discovery {

public:
	explicit Discovery(const string username): hello_message_(username){}

	void start_discovery_service();

	//void send_hello();

	// Return a list of pairs (UserIP, Username)
	void find_users();
	map<string, string> get_online_users() const;

private:
	void start_listening();

	DiscoveryMessage hello_message_;
	map<string, string> online_users_;
	future<void> server_thread_;

	udp_service::UdpClient udp_client_;
};