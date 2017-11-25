#pragma once
#include "Protocol.hpp"
#include "Constants.hpp"
#include "Message.hpp"
#include "Session.hpp"

typedef enum {
	READ_CORRECTLY,					// the packet is received correclty by the server
	URZ_PACKET,						// the packet isn't recognized by the server
	CLD_CONN						// the connection has been closed by peer
} packet_code;

class PacketManager {
private:
	const int buffer_length = MAXBUFL;
	RequestMessage message;
	request_struct request;
public:
	PacketManager();
	packet_code receivePacket(session::conn_ptr connection);
	request_struct get_request_struct();
	bool checkRequest();
	bool sendReply(session::conn_ptr connection, protocol::MessageType::type msgType);			// used to send an ok message
	bool sendReply(session::conn_ptr connection, protocol::MessageType::type msgType, protocol::MessageType::error_type errorType);		// used to send an error message
};
