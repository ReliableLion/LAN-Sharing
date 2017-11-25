#include "PacketManager.hpp"

PacketManager::PacketManager() {}


/**
 * \brief 
 * \param connection : TCP connection for data reading 
 * \return false if the connection has been closed or if the packet is n
 */
packet_code PacketManager::receivePacket(session::conn_ptr connection) {
	char buffer[BUFF_LENGTH];
	char packetType[4];
	int readBytes = 0;

	if (connection->readline(buffer, buffer_length, readBytes) == false) return CLD_CONN;
	
	message.Append(buffer);																		// set the buffer inside the PacketMessage instance
	message.getPacketType(packetType);															// check the packet type	
	int msgType = protocol::MessageType::getMessageType(std::string(packetType));
	
	if (msgType == protocol::MessageType::TYPE::UNDEFINED) return URZ_PACKET;				// if the packet is unrecognizedr
	if (msgType == protocol::MessageType::TYPE::SEND) return READ_CORRECTLY;				// if the packet is send
}

requestStruct PacketManager::get_request_struct() {
	return (request = message.getRequestData());											// set and return the request struct
}

/**
 * \brief : check the reqeust received
 * \return true if the request is ammissible, false otherwise
 */
bool PacketManager::checkRequest() {
	
	if (request.fileSize <= 0) return false;
	if (request.fileName.length() > 256) return false;						// return false if the filename is too long
	//if (request.fileTimestamp)

	return true;
}

bool PacketManager::sendReply(session::conn_ptr connection, protocol::MessageType::TYPE msgType) {
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
bool PacketManager::sendReply(session::conn_ptr connection, protocol::MessageType::TYPE msgType, protocol::MessageType::ERROR_TYPE errorType) {
	int sentByte;

	/*msg.clear();
	msg << protocol::MessageType::getMessageType(msgType);
	return connection->sendall(msg.str().c_str(), msg.str().length(), sentByte);*/
	return false;
}