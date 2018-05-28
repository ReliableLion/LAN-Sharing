#include "PacketManager.hpp"

// this class has only a default constructor
PacketManager::PacketManager() {}

PacketManager::~PacketManager() {}

/**
* \brief
* \param connection : TCP connection used to transfer byte
* \return false if the connection has been closed or if the packet is n
*/
packet_code PacketManager::receive_packet(connection::conn_ptr connection) {
	std::shared_ptr<SocketBuffer> buffer(new SocketBuffer);
	ProtocolMessage req_packet;

	this->connection = connection;

	try {

		if (!connection->read_line(buffer)) return CLSD_CONN;
		
		req_packet.Append(buffer->get_buffer(), buffer->get_size());					// set the buffer inside the PacketMessage instance
																						// check the packet type
		if (req_packet.get_packet_type()) 
		{
			switch (last_message_code = req_packet.get_message_code())
			{
				case protocol::MessageType::UNDEFINED:
				{
					return PACKET_ERR;
				}
				case protocol::MessageType::ERR:
				{
					switch (req_packet.get_error_code()) {
					case MessageType::ERR_1:
					{
						return PACKET_ERR;
					}
					}
				} break;
				case protocol::MessageType::SEND:
				{
					request = req_packet.get_message_request();
					return READ_OK;					// if the packet is sent
				}
				default:
				{
					return PACKET_ERR;
				}
			}
		}
	}
	catch (std::exception &e) {
		// the message parser raised an exception; return a packet error
		return PACKET_ERR;
	}
}

packet_code PacketManager::send_packet(connection::conn_ptr connection, WindowsFileHandler file_handler) {

	std::shared_ptr<SendSocketBuffer> buffer(new SendSocketBuffer);
	ProtocolMessage req_packet;

	FILETIME ftWrite;

	// Retrieve the file times for the file.
	if (!file_handler.get_file_time(nullptr, nullptr, &ftWrite)) return PACKET_ERR;

	RequestMessage request_message(file_handler.get_file_size(), ftWrite, file_handler.get_filename());
	buffer->replace(request_message.get_packet_data().c_str(), strlen(request_message.get_packet_data().c_str()));

	// TODO Check correctness about protocol message to send

	this->connection = connection;

	if (connection->send_data(buffer)) return READ_OK;

	return PACKET_ERR;
}


/**
* \brief
* \return the request struct associated to thee request received by the server
*/
request_struct PacketManager::get_request() {
	return 	request;
}

bool PacketManager::send_reply(connection::conn_ptr connection, protocol::MessageType::message_code msgType) {
	ProtocolMessage res_packet(msgType);

	res_packet.prepare_outgoing_packet();
	// TODO in this part the class must forward the packet to the end point
	return false;
}

/**
* \brief
* \param connection
* \param msgType
* \param errorType
* \return
*/
bool PacketManager::send_error(connection::conn_ptr connection, protocol::MessageType::error_code errorType) {
	int sentByte;
	ProtocolMessage res_packet(errorType);

	// TODO in this part the class must forward the packet to the end point
	return false;
}