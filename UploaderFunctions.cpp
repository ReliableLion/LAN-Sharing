#include "pch.h"

extern "C" __declspec(dllexport) BOOL send_file(const char* address, const char* username, const char* file_path, char* requestIDBuff, bool directory);
extern "C" __declspec(dllexport) BOOL is_terminated(const char* id);
extern "C" __declspec(dllexport) BOOL transferred_correctly(const char* id);
extern "C" __declspec(dllexport) void stop_upload(const char* id);

RequestHandler request_handler;

BOOL send_file(const char* address, const char* username, const char* file_path, char* requestIDBuff, bool directory) {

	const auto destination_user = user(string(username), string(address));

	const auto request = user_request(destination_user, string(file_path));

	return request_handler.send_request(request, requestIDBuff, directory);
}

BOOL is_terminated(const char* id){

	return request_handler.is_terminated(id);
}

BOOL transferred_correctly(const char* id){

	if(request_handler.is_terminated(id))
		return request_handler.get_result(id);

	return false;
}

void stop_upload(const char* id){
	request_handler.cancel_request(id);
}