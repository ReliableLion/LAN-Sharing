#pragma once

#include <sstream>

#include "Protocol.hpp"
#include "Constants.hpp"
#include "Message.hpp"
#include "Connection.hpp"
#include "WindowsFileHandler.hpp"

enum packet_code {
	READ_OK,						// the packet is received correclty by the server
	PACKET_ERR,						// the packet isn't recognized by the server
	CLSD_CONN,						// the connection has been closed by peer
};

class PacketManager {
	const int buffer_length = MAXBUFL;
	request_struct request;
	connection::conn_ptr connection;

	protocol::MessageType::message_code last_message_code;
	protocol::MessageType::message_code last_error_code;

public:
	PacketManager();
	~PacketManager();
	packet_code receivePacket(connection::conn_ptr connection);
	packet_code send_packet(connection::conn_ptr connection, WindowsFileHandler file_handler);
	request_struct get_request();
	bool send_reply(connection::conn_ptr connection, protocol::MessageType::message_code msgType);			// used to send an ok message
	bool send_error(connection::conn_ptr connection, protocol::MessageType::error_code errorType);			// used to send an error message
};


