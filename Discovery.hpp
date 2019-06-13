#pragma once
#include "pch.h"
#include <future>

using namespace std;

class Discovery {

public:
	explicit Discovery(const string username="User"): hello_message_(username){exit_udp.store(false); hide_me_.store(false);}

	void start_discovery_service(std::string username, std::string image_name);
	~Discovery();

	udp_service::UdpServer udp_server_;

	void find_users();

	BOOL stop_discovery_service();

	void hide_me() { hide_me_.store(true); }
	void go_online() { hide_me_.store(false); }

	void set_username(std::string username);
	void set_user_image(std::string image_name);

private:
	void start_listening();

	void start_udp_server();

	DiscoveryMessage hello_message_;
	DiscoveryMessage image_message;

	std::thread discovery_thread_;

	std::atomic<bool> hide_me_;
	std::atomic<bool> exit_udp;

	udp_service::UdpClient udp_client_;
};