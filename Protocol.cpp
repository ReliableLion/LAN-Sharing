#include "Protocol.hpp"

protocol::message_code protocol::MessageType::get_message_type(std::string msg_type) {

	if (MESSAGE_TYPE_MAP.at(msg_type) == SEND)
		return SEND;
	if (MESSAGE_TYPE_MAP.at(msg_type) == OK)
		return OK;
	if (MESSAGE_TYPE_MAP.at(msg_type) == ERR)
		return ERR;

	return UNDEFINED;
}

std::string protocol::MessageType::get_message_type(protocol::message_code msg_type) {
	for (auto it = std::begin(MESSAGE_TYPE_MAP); it != std::end(MESSAGE_TYPE_MAP); ++it)
		if (it->second == msg_type)
			return it->first;

	return std::string("");
}

protocol::error_code protocol::MessageType::get_error_type(std::string error_code) {

	if (ERROR_TYPE_MAP.at(error_code) == SEND)
		return ERR_1;
	else
		// TODO check the error to be returned
		return ERR_1;

}

std::string protocol::MessageType::get_error_type(protocol::error_code error_code) {
	for (auto it = std::begin(ERROR_TYPE_MAP); it != std::end(ERROR_TYPE_MAP); ++it)
		if (it->second == error_code)
			return it->first;

	return std::string("");
}

const std::map<std::string, int> protocol::MessageType::MESSAGE_TYPE_MAP = {
	std::make_pair(std::string("SEND"), 0), // Used when we want to send a file
	std::make_pair(std::string("+OK "), 1), // Used when the server accept the request
	std::make_pair(std::string("-ERR"), 2)  // Used to signal an error
};


const std::map<std::string, int> protocol::MessageType::ERROR_TYPE_MAP = {
	std::make_pair(std::string("ERR_1"), 0)
};