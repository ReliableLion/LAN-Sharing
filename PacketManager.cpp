#include "PacketManager.hpp"
#include "Exceptions.hpp"
#include <iostream>


/*
 * return the information about the status code of the packet
 */
ProtocolMessage PacketManager::receive_packet() const {
    std::shared_ptr<SocketBuffer> buffer(new SocketBuffer);
    ProtocolMessage request_packet;


    if (!connection_->read_line(buffer)) throw ConnectionCloseException();

    //  TODO controllare se il buffer finisce con il \r\n
    request_packet.append(buffer->get_remaining_data(),
                          buffer->get_size());

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
bool PacketManager::send_packet(WindowsFileHandler file_handler) const {

    ProtocolMessage req_packet;
    FILETIME ftWrite;

    // Retrieve the file times for the file.
    if (!file_handler.get_file_time(nullptr, nullptr, &ftWrite)) {
        // TODO is not possible to return the ftWrite
        return false;
    }

    ProtocolMessage request_message(file_handler.get_file_size(), ftWrite, file_handler.get_filename());
    send_buffer_->replace(reinterpret_cast<const char*>(request_message.get_packet_data().data()), request_message.get_packet_data().size());

    return connection_->send_data(send_buffer_);
}

/*
 * send a packet with the correspondenet msg type
 */
bool PacketManager::send_packet(const protocol::message_code msg_type) {
	ProtocolMessage response_packet(msg_type);

	send_buffer_->replace(reinterpret_cast<const char*>(response_packet.get_packet_data().data()), response_packet.get_packet_data().size());
	return connection_->send_data(send_buffer_);
}

bool PacketManager::send_error(const protocol::error_code error_type) {
    //int sent_byte;
    ProtocolMessage response_packet(error_type);

    std::shared_ptr<SendSocketBuffer> buffer(new SendSocketBuffer);
    // TODO da sistemare
    //buffer->replace(response_packet.get_packet_data(), sizeof(response_packet.get_packet_data()));
    return false;
}