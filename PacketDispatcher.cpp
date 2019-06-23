#include "pch.h"

using namespace connection;

PacketDispatcher::PacketDispatcher(connection_ptr connection) : connection_(connection) {
	//SocketBuffer buffer1, buffer2;
	send_buffer_ = std::make_shared<SocketBuffer>();
	recv_buffer_ = std::make_shared<SocketBuffer>();
}

/*
 * return the information about the status code of the packet11
 */
ProtocolMessage PacketDispatcher::receive_packet() const {
    ProtocolMessage request_packet;

	if (!connection_->read_line(recv_buffer_)) {
		if (!connection_->get_connection_status()) {
			throw ConnectionCloseException();
		} else {
			throw PacketFormatException();
		}
	}

    // check if the buffer finish with /r/n
    request_packet.append(recv_buffer_->read_from_buffer(), recv_buffer_->get_remaining_bytes());
	recv_buffer_->read_all_bytes();

	// compute the message type and the message body
	request_packet.compute_packet_type();

	if (request_packet.get_message_code() != protocol::undefined)  {
		request_packet.compute_send_request();
	} else {
		throw PacketFormatException();								// if the packet type is undefined
	}
	
    return request_packet;
}

//        // check the packet type
//        request_packet.compute_packet_type();
//
//        switch (last_message_code = request_packet.get_message_code()) {
//            case protocol::UNDEFINED: {
//                return PACKET_ERR;
//            }
//            case protocol::ERR: {
////                switch (request_packet.get_error_code()) {
////                    case protocol::ERR_1: {
////                        return
////                    }
////                }
//            }
//                break;
//            case protocol::SEND: {
//                if (request_packet.compute_request()) {
//                    return
//                }
//                request = request_packet.get_message_request();
//                return READ_OK;
//            }
//            default: {
//                return PACKET_ERR;
//            }
//        }
//    }
//    catch (std::exception &e) {
//        // the message parser raised an exception; return a packet error
//        return PACKET_ERR;
//    }

/*
 * SEND file_request
 */
bool PacketDispatcher::send_packet(WindowsFileHandler &file_handler, bool directory) const {

    std::shared_ptr<SocketBuffer> buffer(new SocketBuffer);
    ProtocolMessage req_packet;

    FILETIME ftWrite;

    // Retrieve the file times for the file.
    if (!file_handler.get_file_time(nullptr, nullptr, &ftWrite)) {
        // is not possible to return the ftWrite
        return false;
    }

    ProtocolMessage request_message(file_handler.get_file_size(), ftWrite, file_handler.get_filename(), directory);
	send_buffer_->replace(reinterpret_cast<const char*>(request_message.get_packet_data().data()), request_message.get_packet_data().size());

    // Check correctness about protocol message to send
    return connection_->send_data(send_buffer_);
}

/*
 * send a packet with the correspondenet msg type
 */
bool PacketDispatcher::send_packet(const protocol::message_code msg_type) {
	ProtocolMessage response_packet(msg_type);

	send_buffer_->replace(reinterpret_cast<const char*>(response_packet.get_packet_data().data()), response_packet.get_packet_data().size());
	return connection_->send_data(send_buffer_);
}

/*
 * send a packet with an error message
 */ 
bool PacketDispatcher::send_packet(const protocol::error_code error_type) {
    ProtocolMessage response_packet(error_type);

    send_buffer_->replace(reinterpret_cast<const char*>(response_packet.get_packet_data().data()), sizeof(response_packet.get_packet_data()));
	return connection_->send_data(send_buffer_);
}