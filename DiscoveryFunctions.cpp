#include "pch.h"

extern "C" __declspec(dllexport) void SaveDelegate(MANAGED_CALLBACK callback);

MANAGED_CALLBACK saved_callback;
void SaveDelegate(MANAGED_CALLBACK callback) {
	saved_callback = callback;
}

extern "C" __declspec(dllexport) BOOL start_discovery_service(std::string username, std::string image_path);
extern "C" __declspec(dllexport) BOOL set_ip_address(std::string ip_address);
extern "C" __declspec(dllexport) VOID hide_me();
extern "C" __declspec(dllexport) VOID go_online();

Discovery discovery_service;

BOOL start_discovery_service(std::string username, std::string image_path) {
	try{
		discovery_service.start_discovery_service(saved_callback, std::move(username), std::move(image_path));
		cout << "Started UDP server" << endl;
	}catch (std::exception &e) {
		std::cout << "Exception thrown by UDP server: " << WSAGetLastError() << " " << GetLastError() << std::endl;
		return FALSE;
	}

	return TRUE;
}

void hide_me() {
	discovery_service.hide_me();
}

void go_online() {
	discovery_service.go_online();
}

BOOL set_ip_address(std::string ip_address) {
	if(ip_address.size() > INET_ADDRSTRLEN)
		return false;

	discovery_service.set_local_ip_address(ip_address);
	return true;
}
