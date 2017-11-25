#pragma once
#include "Protocol.hpp"
#include "Constant.hpp"
#include "Message.hpp"
#include "Session.hpp"
#include <sstream>

typedef enum {
	READ_CORRECTLY,					// the packet is received correclty by the server
	URZ_PACKET,						// the packet isn't recognized by the server
	CLD_CONN						// the connection has been closed by peer
} packet_code;

class PacketManager {
private:
	const int buffer_length = BUFF_LENGTH;
	MessageType message;
public:
	PacketManager();
	packet_code receivePacket(session::conn_ptr connection);
	request_struct 
	bool sendReply(session::conn_ptr connection, protocol::MessageType::TYPE msgType);			// used to send an ok message
	bool sendReply(session::conn_ptr connection, protocol::MessageType::TYPE msgType, protocol::MessageType::ERROR_TYPE errorType);		// used to send an error message
};


