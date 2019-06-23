#include "pch.h"

protocol::message_code protocol::MessageType::get_message_type(const std::string msg_type) {

	auto it = MESSAGE_TYPE_MAP.find(msg_type);

	if(it != MESSAGE_TYPE_MAP.end()) {
		return MESSAGE_TYPE_MAP.find(msg_type)->second;
	}

	return undefined;
}

std::string protocol::MessageType::get_message_type(const protocol::message_code msg_type) {
	for (auto it = std::begin(MESSAGE_TYPE_MAP); it != std::end(MESSAGE_TYPE_MAP); ++it)
		if (it->second == msg_type)
			return it->first;

	return std::string("");
}

protocol::error_code protocol::MessageType::get_error_type(const std::string error_code) {

	auto it = ERROR_TYPE_MAP.find(error_code);

	if(it != ERROR_TYPE_MAP.end()) {
		return ERROR_TYPE_MAP.find(error_code)->second;
	}

	return err_1;
}

std::string protocol::MessageType::get_error_type(const protocol::error_code error_code) {
	for (auto it = std::begin(ERROR_TYPE_MAP); it != std::end(ERROR_TYPE_MAP); ++it)
		if (it->second == error_code)
			return it->first;

	return std::string("");
}

std::map<std::string, protocol::message_code> protocol::MessageType::MESSAGE_TYPE_MAP = {
	std::make_pair(std::string("SEND"), send), // Used when we want to send a file
	std::make_pair(std::string("+DIR"), dir),  // Used to signal an error
	std::make_pair(std::string("+OK "), ok), // Used when the server accept the request
	std::make_pair(std::string("-ERR"), err)  // Used to signal an error
};


std::map<std::string, protocol::error_code> protocol::MessageType::ERROR_TYPE_MAP = {
	std::make_pair(std::string("ERR_1"), err_1)
};