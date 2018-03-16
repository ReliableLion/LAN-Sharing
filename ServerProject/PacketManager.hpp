#pragma once
#include "Protocol.hpp"
#include "Constants.hpp"
#include "Message.hpp"
#include "Session.hpp"
#include <sstream>

enum packet_code {
	READ_OK,						// the packet is received correclty by the server
	PACKET_ERR,						// the packet isn't recognized by the server
	CLSD_CONN						// the connection has been closed by peer
};

class PacketManager {
	const int buffer_length = BUFF_LENGTH;
	RequestMessage message_handler;
	request_struct request;
public:
	PacketManager();
	packet_code receivePacket(session::conn_ptr connection);
	request_struct get_request_struct();
	bool check_request();
	bool send_reply(session::conn_ptr connection, protocol::MessageType::type msgType);			// used to send an ok message
	bool send_reply(session::conn_ptr connection, protocol::MessageType::type msgType, protocol::MessageType::error_type errorType);		// used to send an error message
};


