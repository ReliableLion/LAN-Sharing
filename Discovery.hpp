#pragma once
#include "UDPService.hpp"
#include "Message.hpp"
#include <future>

using namespace std;

class Discovery {

public:
	explicit Discovery(const string username): hello_message_(username){}

	void start_discovery_service();
	~Discovery();

	udp_service::UdpServer udp_server_;

	//void send_hello();

	// Return a list of pairs (UserIP, Username)
	void find_users();
	void set_local_ip_address(std::string ip_address);
	map<string, string> get_online_users();
	map<string, string> get_user_image();

	boolean stop_discovery_service();

	void hide_me() { hide_me_ = true; };
	void go_online() { hide_me_ = false; };

	void set_username(std::string username);
	void set_user_image(std::string image_path);

private:
	void start_listening();

	void start_udp_server();

	DiscoveryMessage hello_message_;
	DiscoveryMessage image_message;

	std::string user_image_path;
	map<string, string> online_users_;
	map<string, string> users_image;

	std::thread discovery_thread_;

	boolean hide_me_ = false;
	std::mutex lock_map1_;
	std::mutex lock_map2_;
	std::atomic<bool> exit_udp = false;

	udp_service::UdpClient udp_client_;
};