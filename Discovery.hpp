#pragma once
#include "pch.h"
#include <future>

using namespace std;

class Discovery {

public:
	explicit Discovery(const string username="User"): hello_message_(username){}

	void start_discovery_service(EXCEPTION_CALLBACK callback, DISCOVERY_MANAGED_CALLBACK d_callback, std::string username, std::string image_name);
	~Discovery();

	udp_service::UdpServer udp_server_;

	void find_users();
	void set_local_ip_address(std::string ip_address);

	BOOL stop_discovery_service();

	void hide_me() { hide_me_ = true; };
	void go_online() { hide_me_ = false; };

	void set_username(std::string username);
	void set_user_image(std::string image_path);

private:
	void start_listening();

	void start_udp_server(EXCEPTION_CALLBACK callback);

	DiscoveryMessage hello_message_;
	DiscoveryMessage image_message;
	DISCOVERY_MANAGED_CALLBACK discovery_callback;

	std::thread discovery_thread_;

	BOOL hide_me_ = false;
	std::atomic<bool> exit_udp = false;

	udp_service::UdpClient udp_client_;
};