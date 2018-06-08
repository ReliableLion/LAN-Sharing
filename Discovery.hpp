#pragma once
#include "UDPService.hpp"
#include "Message.hpp"
#include <future>

using namespace std;

class Discovery {

public:
	explicit Discovery(const string username): hello_message_(username){}

	void start_discovery_service();

	udp_service::UdpServer udp_server_;

	//void send_hello();

	// Return a list of pairs (UserIP, Username)
	void find_users();
	map<string, string> get_online_users() const;
	boolean stop_discovery_service();

	void hide_me() { hide_me_ = true; };
	void go_online() { hide_me_ = false; };

private:
	void start_listening();

	DiscoveryMessage hello_message_;
	map<string, string> online_users_;
	std::thread discovery_thread_;

	boolean hide_me_ = false;

	udp_service::UdpClient udp_client_;
};