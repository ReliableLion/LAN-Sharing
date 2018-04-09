#pragma once

#include <sstream>
#include <vector>
#include <windows.h>

#include "Protocol.hpp"
#include "Exceptions.hpp"
#include "Constants.hpp"

#define _MAX_FILENAME_LENGTH_ 256

using namespace protocol;

typedef struct 
{
	__int64 file_size;
	FILETIME file_timestamp;
	std::string file_name;
} request_struct;

class Message
{
protected:
	std::string message_body_ = "";
	std::stringstream stream_;
	const std::string end_message_ = "\r\n";
	size_t message_size_;

	//Message Packet Buffer
	std::vector<int8_t> m_buffer_; 

public:
	Message();
	Message(const char * buffer, const int size); //Will use existing allocated buffer and create packet from it
												  //Packet(const Packet & p); //Will allocate new buffer but copy buffer from packet argument
	Message(const MessageType::message_code message_code); //Used for when sending a packet that only contains a packet type (Ex. End of File Packet)
	Message(const MessageType::error_code error);
	void append(const MessageType::message_code mt);
	void append(const MessageType::error_code error);
	void append(const __int64 int64);
	void append(const std::size_t m);
	void append(const Message & m);
	void append(const std::string & str);
	void append(const char * buffer, const int size); //Will use existing allocated buffer and create packet from it
};

class ProtocolMessage : public Message
{
	protocol::MessageType::message_code message_code_;
	protocol::MessageType::error_code error_code_;
	request_struct request_body_;
	ULARGE_INTEGER time_stamp_;

	int const min_size_request_ = (4 + (2 * sizeof(__int64)) + 1);
	int const max_size_request_ = (4 + (2 * sizeof(__int64)) + _MAX_FILENAME_LENGTH_);

	// private methods
	void prepare_send_message();
public:
	ProtocolMessage(__int64 file_size, FILETIME file_timestamp, std::string file_name);	// This is used to create a RequestMessage, which it's supposed will be sent
	ProtocolMessage(protocol::MessageType::message_code);
	ProtocolMessage(protocol::MessageType::error_code error);							// this constructor is used to build up an error message
	ProtocolMessage() {};																// This is used to create an empty RequestMessage, which it's supposed will be received

	/*__int64 ProtocolMessage::get_file_size() const {
		return this->requestBody.file_size;
	}

	FILETIME ProtocolMessage::get_file_time_stamp() const {
		return this->requestBody.file_timestamp;
	}

	std::string ProtocolMessage::get_file_name() const {
		return this->requestBody.file_name;
	}*/

	bool get_packet_type();
	void prepare_outgoing_packet();
	std::vector<int8_t> get_packet_data() const;

	// this method is used to convert the buffer receive into a reuqest
	bool convert_incoming_packet();

	request_struct get_message_request() { return request_body_; };
	protocol::MessageType::message_code get_message_code() { return message_code_; };
	protocol::MessageType::error_code get_error_code() { return error_code_; }
};

class discovery_message : public Message
{

public:
	explicit discovery_message(const std::string username)
	{
		append(HELLO_MSG);
		append(username);
		append(end_message_);
	}

	discovery_message()
	{
		append(DISCOVERY_MSG);
		message_body_.append(end_message_);
	}

	std::string discovery_message::getUsername(char* username);
	std::string discovery_message::get_packet_type();
};
