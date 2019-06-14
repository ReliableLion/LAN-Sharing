#include "pch.h"

extern "C" __declspec(dllexport) BOOL start_discovery_service(const char* address, const char* username, const char* file_path);
extern "C" __declspec(dllexport) BOOL is_terminated(const char* address, const char* username, const char* file_path);
extern "C" __declspec(dllexport) BOOL transferred_correctly(const char* address, const char* username, const char* file_path);

RequestHandler request_handler;

BOOL start_discovery_service(const char* address, const char* username, const char* file_path) {

	const auto destination_user = user(string(username), string(address));

	const auto request = user_request(destination_user,string(file_path));

	return request_handler.send_request(request);
}

BOOL is_terminated(const char* address, const char* username, const char* file_path){
	
	const auto destination_user = user(string(username), string(address));

	const auto request = user_request(destination_user,string(file_path));

	return request_handler.is_terminated(request);
}

BOOL transferred_correctly(const char* address, const char* username, const char* file_path){
	
	const auto destination_user = user(string(username), string(address));

	const auto request = user_request(destination_user,string(file_path));

	if(request_handler.is_terminated(request))
		return request_handler.get_result(request);

	return false;
}