#pragma once
#include <WinSock2.h>
#include <windows.h>
#include <sstream>
#include "Protocol.hpp"
#include <vector>
#include "Constants.hpp"

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
	explicit message(const MessageType::type m); //Used for when sending a packet that only contains a packet type (Ex. End of File Packet)
	void Append(const MessageType::type mt);
	void Append(const __int64 int64);
	void Append(const std::size_t m);
	void Append(const message & m);
	void Append(const std::string & str);
	void Append(const char * buffer, const int size); //Will use existing allocated buffer and create packet from it
	void clear();


	std::vector<int8_t> m_buffer; //Message Packet Buffer

protected:
	std::string messageBody = "";
	std::stringstream stream;
	const std::string endMessage = "\r\n";
	size_t messageSize;

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
		Append(endMessage);
	}

	discovery_message();

	std::string discovery_message::get_username(char* username);

	std::string discovery_message::get_packet_type();

	std::string discovery_message::get_message_body();
};