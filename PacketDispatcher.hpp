#pragma once

#include "Protocol.hpp"
#include "Message.hpp"
#include "Connection.hpp"
#include "SocketBuffer.hpp"
#include "WindowsFileHandler.hpp"

class PacketDispatcher {
private:
    //const int buffer_length = MAXBUFL;
    //request_struct request;
    connection::connection_ptr connection_;
	std::shared_ptr<SocketBuffer> send_buffer_;
	std::shared_ptr<SocketBuffer> recv_buffer_;

    //protocol::message_code last_message_code;
    //protocol::message_code last_error_code;

public:
	explicit PacketDispatcher(connection::connection_ptr connection);

	~PacketDispatcher() {};

    ProtocolMessage receive_packet() const;

    bool send_packet(WindowsFileHandler &file_handler) const;

	bool send_packet(protocol::message_code msg_type);           // used to send an ok message

	bool send_packet(protocol::error_code error_type);            // used to send an error message
};


