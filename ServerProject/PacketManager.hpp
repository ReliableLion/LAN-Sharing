#pragma once
#include "Protocol.hpp"
#include "Session.hpp"
#include <sstream>

class PacketManager {
private:
	std::stringstream msg;
public:
	PacketManager();
	void sendPacket();
	void receivePacket();
	void receivecRequest();
	bool sendReply(session::conn_ptr conn, protocol::MessageType::TYPE msgType);			// used to send an ok message
	bool sendReply(session::conn_ptr conn, protocol::MessageType::TYPE msgType, protocol::MessageType::ERROR_TYPE errorType);		// used to send an error message
};


