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
	std::shared_ptr<SendSocketBuffer> send_buffer_;

    //protocol::message_code last_message_code;
    //protocol::message_code last_error_code;

public:
	explicit PacketManager(connection::conn_ptr connection) : connection_(connection) {
		SendSocketBuffer buffer;
		send_buffer_ = std::make_shared<SendSocketBuffer>(buffer);
	}

	~PacketManager() {};

    ProtocolMessage receive_packet() const;

    //request_struct get_request();
    bool send_packet(WindowsFileHandler &file_handler) const;

	bool send_packet(protocol::message_code msg_type);            // used to send an ok message

	bool send_error(protocol::error_code error_type);            // used to send an error message
};


