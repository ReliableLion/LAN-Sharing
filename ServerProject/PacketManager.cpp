#include "PacketManager.hpp"

PacketManager::PacketManager() {}


/**
 * \brief 
 * \param connection : TCP connection for data reading 
 * \return false if the connection has been closed or if the packet is n
 */
packet_code PacketManager::receivePacket(session::conn_ptr connection) {
	char buffer[BUFF_LENGTH];
	char packetType[5];
	int readBytes = 0;

	if (connection->readline(buffer, buffer_length, readBytes) == false) return CLD_CONN;
	
	message.Append(buffer, readBytes);																		// set the buffer inside the PacketMessage instance
	message.get_packet_type(packetType);															// check the packet type	
	try {
		int msgType = protocol::MessageType::getMessageType(std::string(packetType));

		if (msgType == protocol::MessageType::type::undefined) return URZ_PACKET;				// if the packet is unrecognizedr
		if (msgType == protocol::MessageType::type::send) return READ_CORRECTLY;				// if the packet is send}
	
	} catch (std::exception e) {
		return URZ_PACKET;
	}
}

request_struct PacketManager::get_request_struct() {
	return (request = message.get_request_data());											// set and return the request struct
}

/**
 * \brief : check the reqeust received
 * \return true if the request is ammissible, false otherwise
 */
bool PacketManager::check_request() {
	
	if (request.file_size <= 0) return false;
	if (request.file_name.length() > 256) return false;						// return false if the filename is too long
	//if (request.fileTimestamp)

	return true;
}

bool PacketManager::send_reply(session::conn_ptr connection, protocol::MessageType::type msgType) {
	int sentByte;

	/*msg.clear();
	msg << protocol::MessageType::getMessageType(msgType);
	return connection->sendall(msg.str().c_str(), msg.str().length(), sentByte);*/
	return false;
}

/**
 * \brief 
 * \param connection 
 * \param msgType 
 * \param errorType 
 * \return 
 */
bool PacketManager::send_reply(session::conn_ptr connection, protocol::MessageType::type msgType, protocol::MessageType::error_type errorType) {
	int sentByte;

	/*msg.clear();
	msg << protocol::MessageType::getMessageType(msgType);
	return connection->sendall(msg.str().c_str(), msg.str().length(), sentByte);*/
	return false;
}