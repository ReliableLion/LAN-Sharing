#pragma once

#include <sstream>

#include "Protocol.hpp"
#include "Constants.hpp"
#include "Message.hpp"
#include "Connection.hpp"
#include "SocketBuffer.hpp"
#include "WindowsFileHandler.hpp"

enum packet_code {
    ERR_OK,
	SEND_OK,
	READ_OK,						// the packet is received correclty
	PACKET_ERR,						// the packet isn't recognized
	CLOSED_CONNECTION,				// the connection has been closed by peer
};

class PacketManager {
	//const int buffer_length = MAXBUFL;
	//request_struct request;
	connection::conn_ptr connection;

	//protocol::message_code last_message_code;
	//protocol::message_code last_error_code;

public:
	PacketManager(connection::conn_ptr connection);
	~PacketManager();
	ProtocolMessage receive_packet();
	//request_struct get_request();
	bool send_packet(WindowsFileHandler file_handler);
	bool send_reply(protocol::message_code msgType);			// used to send an ok message
	bool send_error(protocol::error_code errorType);			// used to send an error message
};


