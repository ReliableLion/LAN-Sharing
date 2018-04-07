#include "Protocol.hpp"

/**
 * \brief 
 * \param msg_type represent the message in string format 
 * \return the enum type of the message
 */
protocol::MessageType::message_code protocol::MessageType::get_message_type(std::string msg_type) {

	if (message_type.at(msg_type) == SEND)
		return SEND;
	if (message_type.at(msg_type) == OK)
		return OK;
	if (message_type.at(msg_type) == ERR)
		return ERR;
	
	return UNDEFINED;
}

/**
 * \brief 
 * \param msg_type : enum type of the message 
 * \return the string that represent the message code
 */
std::string protocol::MessageType::get_message_type(protocol::MessageType::message_code msg_type) {
	for (auto it = std::begin(message_type); it != std::end(message_type); ++it)
		if (it->second == msg_type)
			return it->first;

	return std::string("");
}

protocol::MessageType::error_code protocol::MessageType::get_error_type(std::string error_code) {

	if (error_type.at(error_code) == SEND)
		return ERR_1;
	else
		// TODO check the error to be returned
		return ERR_1;

}

std::string protocol::MessageType::get_error_type(protocol::MessageType::error_code error_code) {
	for (auto it = std::begin(error_type); it != std::end(error_type); ++it)
		if (it->second == error_code)
			return it->first;

	return std::string("");
}

const std::map<std::string, int> protocol::MessageType::message_type = {
	std::make_pair(std::string("SEND"), 0), // Used when we want to send a file
	std::make_pair(std::string("+OK "), 1), // Used when the server accept the request
	std::make_pair(std::string("-ERR"), 2)  // Used to signal an error
};


const std::map<std::string, int> protocol::MessageType::error_type = {
	std::make_pair(std::string("ERR_1"), 0)
};