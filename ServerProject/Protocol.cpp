#include "Protocol.hpp"

int protocol::MessageType::getMessageType(std::string msgType) {

	if (messageType.at(msgType) == send)
		return send;
	else if (messageType.at(msgType) == ok)
		return ok;
	else if (messageType.at(msgType) == err)
		return err;
	else
		return undefined;
}

std::string protocol::MessageType::getMessageType(int msgType) {

	for (auto it = messageType.begin(); it != messageType.end(); ++it)
		if (it->second == msgType)
			return it->first;

	return NULL;
}

const std::map<std::string, int> protocol::MessageType::messageType = {
	std::make_pair(std::string("SEND"), 0), // Used when we want to send a file
	std::make_pair(std::string("+OK "), 1), // Used when the server accept the request
	std::make_pair(std::string("-ERR"), 2)  // Used to signal an error
};