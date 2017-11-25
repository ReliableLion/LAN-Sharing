#include "Protocol.hpp"

int protocol::MessageType::getMessageType(const std::string msg_type) {

	if (message_type.at(msg_type) == send)
		return send;
	else if (message_type.at(msg_type) == ok)
		return ok;
	else if (message_type.at(msg_type) == err)
		return err;
	else
		return undefined;
}

std::string protocol::MessageType::getMessageType(const int msg_type) {

	for (auto it = message_type.begin(); it != message_type.end(); ++it)
		if (it->second == msg_type)
			return it->first;

	return nullptr;
}

const std::map<std::string, int> protocol::MessageType::message_type = {
	std::make_pair(std::string("SEND"), 0), // Used when we want to send a file
	std::make_pair(std::string("+OK "), 1), // Used when the server accept the request
	std::make_pair(std::string("-ERR"), 2)  // Used to signal an error
};
