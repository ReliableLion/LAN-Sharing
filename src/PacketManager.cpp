#include "PacketManager.hpp"

// this class has only a default constructor
PacketManager::PacketManager(connection::conn_ptr connection) : connection(connection) {}

PacketManager::~PacketManager() {}

/*
 * return the information about the status code of the packet
 */
ProtocolMessage PacketManager::receive_packet() {
    std::shared_ptr<SocketBuffer> buffer(new SocketBuffer);
    ProtocolMessage request_packet;

    // TODO sistemare il closed connection
    if (!connection->read_line(buffer)) //return CLOSED_CONNECTION;

    //  TODO controllare se il buffer finisce con il \r\n
    request_packet.append(buffer->get_buffer(),
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
 * send
 */
bool PacketManager::send_packet(WindowsFileHandler file_handler) {

    std::shared_ptr<SendSocketBuffer> buffer(new SendSocketBuffer);
    ProtocolMessage req_packet;

    FILETIME ftWrite;

    // Retrieve the file times for the file.
    if (!file_handler.get_file_time(nullptr, nullptr, &ftWrite)) {
        // TODO is not possible to return the ftWrite
        return false;
    }

    RequestMessage request_message(file_handler.get_file_size(), ftWrite, file_handler.get_filename());
    buffer->replace(request_message.get_packet_data().c_str(), strlen(request_message.get_packet_data().c_str()));

    // TODO Check correctness about protocol message to send

    return connection->send_data(buffer);
}

/*
 * return the request of the
 */
//request_struct PacketManager::get_request() {
//    return request;
//}

bool PacketManager::send_reply(protocol::message_code msgType) {
    ProtocolMessage res_packet(msgType);

    res_packet.compute_request();
    // TODO in this part the class must forward the packet to the end
    return false;
}

bool PacketManager::send_error(protocol::error_code errorType) {
    int sentByte;
    ProtocolMessage response_packet(errorType);

    std::shared_ptr<SendSocketBuffer> buffer(new SendSocketBuffer);
    // TODO da sistemare
    //buffer->replace(response_packet.get_packet_data(), sizeof(response_packet.get_packet_data()));
    return false;
}