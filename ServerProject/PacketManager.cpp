#include "PacketManager.hpp"

PacketManager::PacketManager() {}


/**
 * \brief 
 * \param connection : TCP connection for data reading 
 * \return false if the connection has been closed or if the packet is n
 */
packet_code PacketManager::receivePacket(session::conn_ptr connection) {
	char buffer[buffer_length];
	char packetType[4];
	Message msg;
	int readBytes = 0;

	if (connection->readline(buffer, readBytes, buffer_length) == false) return CLD_CONN;
	
	msg.getPacketType(packetType);															// check the packet type	
	int msgType = protocol::MessageType::getMessageType(std::string(packetType));
	
	if (msgType == protocol::MessageType::TYPE::UNDEFINED) return URZ_PACKET;				// if the packet is unrecognizedr
	if (msgType == protocol::MessageType::TYPE::SEND) return READ_CORRECTLY;				// if the packet is send
}

bool PacketManager::sendReply(session::conn_ptr connection, protocol::MessageType::TYPE msgType) {
	int sentByte;

	msg.clear();
	msg << protocol::MessageType::getMessageType(msgType);
	return connection->sendall(msg.str().c_str(), msg.str().length(), sentByte);
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

	msg.clear();
	msg << protocol::MessageType::getMessageType(msgType);
	return connection->sendall(msg.str().c_str(), msg.str().length(), sentByte);
}