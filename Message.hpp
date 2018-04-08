#pragma once
#include <WinSock2.h>
#include <windows.h>
#include <sstream>
#include "Protocol.hpp"
#include <vector>
#include "Constants.hpp"
#include <iostream>

using namespace protocol;

typedef struct {
	__int64 file_size;
	FILETIME file_timestamp;
	std::string file_name;
} request_struct;

class message {

public:

	message();
	message(const char * buffer, const int size); //Will use existing allocated buffer and create packet from it
												 //Packet(const Packet & p); //Will allocate new buffer but copy buffer from packet argument
	explicit message(const MessageType::message_code m); //Used for when sending a packet that only contains a packet type (Ex. End of File Packet)
	void Append(const MessageType::message_code mt);
	void Append(const __int64 int64);
	void Append(const std::size_t m);
	void Append(const message & m);
	void Append(const std::string & str);
	void Append(const char * buffer, const int size); //Will use existing allocated buffer and create packet from it
	void Append(const MessageType::error_code mt);
	void clear();


	std::vector<int8_t> m_buffer; //Message Packet Buffer

protected:
	std::string message_body_ = "";
	std::stringstream stream_;
	const std::string end_message_ = "\r\n";
	size_t message_size_;

};

class RequestMessage : public message {

public:

	// This is used to create a RequestMessage, which it's supposed will be sent
	RequestMessage(__int64 fileSize, FILETIME fileTimestamp, std::string fileName);

	// This is used to create an empty RequestMessage, which it's supposed will be received
	RequestMessage() {};

	__int64 RequestMessage::get_file_size() const {
		return this->requestBody.file_size;
	}

	FILETIME RequestMessage::get_file_time_stamp() const {
		return this->requestBody.file_timestamp;
	}

	std::string RequestMessage::get_file_name() const {
		return this->requestBody.file_name;
	}

	void RequestMessage::prepare_message();

	request_struct RequestMessage::get_request_data();
	void RequestMessage::get_packet_type(char* packetType);
	std::vector<int8_t> get_packet_data() const;

private:
	request_struct requestBody;
	ULARGE_INTEGER timeStamp;

};

class discovery_message: public message {

public:
	explicit discovery_message(const std::string username) {
		Append(HELLO_MSG);
		Append(username);
		Append(end_message_);
	}

	discovery_message();

	std::string discovery_message::get_username();

	std::string discovery_message::get_packet_type();

	std::string discovery_message::get_message_body();
};

class ProtocolMessage : public message {
	protocol::MessageType::message_code message_code_;
	protocol::MessageType::error_code error_code_;
	request_struct request_body_;
	ULARGE_INTEGER time_stamp_;

	int const min_size_request_ = (4 + (2 * sizeof(__int64)) + 1);
	int const max_size_request_ = (4 + (2 * sizeof(__int64)) + MAX_FILENAME_LENGTH_);

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

																						std::string ProtocolMessage::get_file_name() const {
																						return this->requestBody.file_name;
																						}*/

	bool ProtocolMessage::get_packet_type();

	void ProtocolMessage::prepare_outgoing_packet();
	std::vector<int8_t> get_packet_data() const;

	// this method is used to convert the buffer receive into a reuqest
	bool ProtocolMessage::convert_incoming_packet();

	request_struct get_message_request() { return request_body_; };
	protocol::MessageType::message_code get_message_code() { return message_code_; };
	protocol::MessageType::error_code get_error_code() { return error_code_; }
};