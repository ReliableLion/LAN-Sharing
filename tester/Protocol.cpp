#include "stdafx.h"

#include "Protocol.hpp"

/**
 * \brief 
 * \param msgType represent the message in string format 
 * \return the enum type of the message
 */
protocol::MessageType::message_code protocol::MessageType::getMessageType(std::string msgType) {

	if (messageType.at(msgType) == SEND)
		return SEND;
	if (messageType.at(msgType) == OK)
		return OK;
	if (messageType.at(msgType) == ERR)
		return ERR;
	
	return UNDEFINED;
}

/**
 * \brief 
 * \param msgType : enum type of the message 
 * \return the string that represent the message code
 */
std::string protocol::MessageType::getMessageType(protocol::MessageType::message_code msgType) {
	for (auto it = std::begin(messageType); it != std::end(messageType); ++it)
		if (it->second == msgType)
			return it->first;

	return std::string("");
}

protocol::MessageType::error_code protocol::MessageType::getErrorType(std::string errorCode) {

	if (errorType.at(errorCode) == SEND)
		return ERR_1;

}

std::string protocol::MessageType::getErrorType(protocol::MessageType::error_code errorCode) {
	for (auto it = std::begin(errorType); it != std::end(errorType); ++it)
		if (it->second == errorCode)
			return it->first;

	return std::string("");
}

const std::map<std::string, int> protocol::MessageType::messageType = {
	std::make_pair(std::string("SEND"), 0), // Used when we want to send a file
	std::make_pair(std::string("+OK "), 1), // Used when the server accept the request
	std::make_pair(std::string("-ERR"), 2)  // Used to signal an error
};


const std::map<std::string, int> protocol::MessageType::errorType = {
	std::make_pair(std::string("ERR_1"), 0)
};