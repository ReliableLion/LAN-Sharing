#include "PacketManager.hpp"

PacketManager::PacketManager() {}

bool PacketManager::sendReply(session::conn_ptr conn, protocol::MessageType::TYPE msgType) {
	int sentByte;

	msg.clear();
	msg << protocol::MessageType::getMessageType(msgType);
	return conn->sendall(msg.str().c_str(), msg.str().length(), sentByte);
}

bool PacketManager::sendReply(session::conn_ptr conn, protocol::MessageType::TYPE msgType, protocol::MessageType::ERROR_TYPE errorType) {
	int sentByte;

	msg.clear();
	msg << protocol::MessageType::getMessageType(msgType);
	return conn->sendall(msg.str().c_str(), msg.str().length(), sentByte);
}