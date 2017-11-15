#include "PacketManager.hpp"

PacketManager::PacketManager() {}

bool PacketManager::sendReply(session::conn_ptr conn, protocol::MessageType::TYPE msgType) {
	msg.clear();
	msg << protocol::MessageType::getMessageType(msgType);
	return conn->sendall(msg.str().c_str(), msg.str().length());
}

bool PacketManager::sendReply(session::conn_ptr conn, protocol::MessageType::TYPE msgType, protocol::MessageType::ERROR_TYPE errorType) {
	msg.clear();
	msg << protocol::MessageType::getMessageType(msgType);
	return conn->sendall(msg.str().c_str(), msg.str().length());
}