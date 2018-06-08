#pragma once

#include "Protocol.hpp"
#include "Message.hpp"
#include "Connection.hpp"
#include "SocketBuffer.hpp"
#include "WindowsFileHandler.hpp"

class PacketManager {
private:
    //const int buffer_length = MAXBUFL;
    //request_struct request;
    connection::conn_ptr connection_;

    //protocol::message_code last_message_code;
    //protocol::message_code last_error_code;

public:
	explicit PacketManager(connection::conn_ptr connection);

    ~PacketManager();

    ProtocolMessage receive_packet() const;

    //request_struct get_request();
    bool send_packet(WindowsFileHandler &file_handler) const;

	static bool send_reply(protocol::message_code msg_type);            // used to send an ok message
	static bool send_error(protocol::error_code error_type);            // used to send an error message
};


