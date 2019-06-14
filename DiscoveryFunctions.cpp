#include "pch.h"
#include <utility>

extern "C" __declspec(dllexport) BOOL start_discovery_service(const char* username, const char* image_name);
extern "C" __declspec(dllexport) VOID hide_me();
extern "C" __declspec(dllexport) VOID go_online();

extern "C" __declspec(dllexport) VOID set_username(const char* username);
extern "C" __declspec(dllexport) VOID set_avatar(const char* image_name);
extern "C" __declspec(dllexport) VOID start_collection();

Discovery discovery_service;

BOOL start_discovery_service(const char* username, const char* image_name) {

	try{
		discovery_service.start_discovery_service(string(username), string(image_name));
		cout << "Started UDP server" << endl;
	}catch (std::exception &e) {
		std::cout << "Exception thrown by UDP server: " << WSAGetLastError() << " " << GetLastError() << std::endl;
		return FALSE;
	}

	return TRUE;
}

VOID start_collection() {
	discovery_service.find_users();
}

void hide_me() {
	discovery_service.hide_me();
}

void go_online() {
	discovery_service.go_online();
}

VOID set_username(const char* username) {
	discovery_service.set_username(std::move(username));
}

VOID set_avatar(const char* image_name) {
	discovery_service.set_user_image(std::move(image_name));
}