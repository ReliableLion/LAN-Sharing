#include "Protocol.hpp"

int protocol::MessageType::getMessageType(std::string msgType) {

	if (messageType.at(msgType) == send)
		return send;
	if (messageType.at(msgType) == ok)
		return ok;
	if (messageType.at(msgType) == err)
		return err;
	
	return undefined;
}

std::string protocol::MessageType::getMessageType(int msgType) {

	for (auto it = std::begin(messageType); it != std::end(messageType); ++it)
		if (it->second == msgType)
			return it->first;

	return std::string("");
}

const std::map<std::string, int> protocol::MessageType::messageType = {
	std::make_pair(std::string("SEND"), 0), // Used when we want to send a file
	std::make_pair(std::string("+OK "), 1), // Used when the server accept the request
	std::make_pair(std::string("-ERR"), 2)  // Used to signal an error
};