#include "pch.h"

extern "C" __declspec(dllexport) void SaveDelegate(EXCEPTION_CALLBACK callback);

EXCEPTION_CALLBACK saved_callback;
void SaveDelegate(EXCEPTION_CALLBACK callback) {
	saved_callback = callback;
}

extern "C" __declspec(dllexport) void SaveUserDelegate(DISCOVERY_MANAGED_CALLBACK callback);

DISCOVERY_MANAGED_CALLBACK discovery_callback;
void SaveUserDelegate(DISCOVERY_MANAGED_CALLBACK callback) {
	discovery_callback = callback;
}

extern "C" __declspec(dllexport) BOOL start_discovery_service(std::string username, std::string image_name);
extern "C" __declspec(dllexport) BOOL set_ip_address(std::string ip_address);
extern "C" __declspec(dllexport) VOID hide_me();
extern "C" __declspec(dllexport) VOID go_online();
extern "C" __declspec(dllexport) VOID start_collection();


VOID start_collection() {
	discovery_callback("192.168.1.102", "Davide", "");
	discovery_callback("192.168.1.102", "", "man");

	discovery_callback("192.168.1.103", "Davide2", "");
	discovery_callback("192.168.1.103", "", "woman");

	discovery_callback("192.168.1.105", "Davide4", "");
	discovery_callback("192.168.1.105", "", "businessman");
}

Discovery discovery_service;

BOOL start_discovery_service(std::string username, std::string image_name) {
	try{
		discovery_service.start_discovery_service(saved_callback, discovery_callback, std::move(username), std::move(image_name));
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


