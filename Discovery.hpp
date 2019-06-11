#pragma once
#include "pch.h"
#include <future>

using namespace std;

class Discovery {

public:
	explicit Discovery(const string username="User"): hello_message_(username){}

	void start_discovery_service(MANAGED_CALLBACK callback, std::string username, std::string image_path);
	~Discovery();

	udp_service::UdpServer udp_server_;

	//void send_hello();

	// Return a list of pairs (UserIP, Username)
	void find_users();
	void set_local_ip_address(std::string ip_address);
	map<string, string> get_online_users() const;
	BOOL stop_discovery_service();

	void hide_me() { hide_me_ = true; };
	void go_online() { hide_me_ = false; };

	void set_username(std::string username);
	void set_user_image(std::string image_path);

private:
	void start_listening();

	void start_udp_server(MANAGED_CALLBACK callback);

	DiscoveryMessage hello_message_;
	DiscoveryMessage image_message;

	std::string user_image_path;
	map<string, string> online_users_;
	map<string, string> users_image;

	std::thread discovery_thread_;

	BOOL hide_me_ = false;

	udp_service::UdpClient udp_client_;
};