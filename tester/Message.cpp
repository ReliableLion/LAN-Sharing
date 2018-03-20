#include "stdafx.h"

#include "Message.hpp"
#include <cstdint> //Required to use std::int32_t

Message::Message() : messageSize(0) {
}

Message::Message(const char * data_buffer, const int size) {

	append(data_buffer, size);
}

Message::Message(const MessageType::message_code message_code) {

	append(message_code);
}

Message::Message(const MessageType::error_code error) {
	
	append(error);
}


void Message::append(const char * data_buffer, const int size) {

	m_buffer.insert(m_buffer.end(), data_buffer, data_buffer + size);
}

void Message::append(const std::string & str) {

	append(str.c_str(), str.size());
}

void Message::append(const Message & p) { //Allocate new block for buffer

	append(reinterpret_cast<const char*>(&p.m_buffer), p.m_buffer.size());
}

void Message::append(const __int64 int64) {

	__int64 val = htonll(int64);
	append(reinterpret_cast<const char*>(&val), sizeof(__int64));
}

void Message::append(const MessageType::message_code mt) {
	std::string res_code_msg = MessageType::getMessageType(mt);

	if(res_code_msg.empty()) 
		append(static_cast<std::string>(res_code_msg));
		// TODO check if should be raised an exception
}

void Message::append(const MessageType::error_code mt) {
	std::string res_code_msg = MessageType::getErrorType(mt);

	if (res_code_msg.empty())
		append(static_cast<std::string>(res_code_msg));
	// TODO check if should be raised an exception
}

void Message::append(const std::size_t s) {

	append(static_cast<__int64>(s));
}





/*
 *					REQUEST MESSAGE IMPLEMENTATION
 */



/**
 * \brief this constructor create a new request message
 * \param file_size: size of the file 
 * \param file_timestamp: last modified timestamp
 * \param file_name: name of the file
 */
ProtocolMessage::ProtocolMessage(const __int64 file_size, const FILETIME file_timestamp, const std::string file_name) {

	// first append the type of the message
	this->messageCode = MessageType::SEND;

	// insert the request information
	this->requestBody.file_size = file_size;
	this->requestBody.file_timestamp = file_timestamp;
	this->requestBody.file_name = file_name;
}

ProtocolMessage::ProtocolMessage(protocol::MessageType::message_code message_code) {
	this->messageCode = message_code;
}

ProtocolMessage::ProtocolMessage(protocol::MessageType::error_code error) {
	this->messageCode = MessageType::ERR;
	this->errorCode = error;
}

/**
 * \brief 
 * \return 
 */
bool ProtocolMessage::convert_incoming_packet() {

	// TODO check if the buffer contain the exact size of data
	if(m_buffer.size() < _min_size_request_  ||  
		m_buffer.size() > _max_size_request_)
	{
		return false;
	}

	const std::vector<int8_t>::const_iterator begin = m_buffer.begin() + 4;

	std::vector<int8_t>::const_iterator first = begin;
	std::vector<int8_t>::const_iterator last = first + sizeof(__int64);
	std::vector<int8_t> file_size(first, last);

	memcpy(&requestBody.file_size, file_size.data(), file_size.size());
	requestBody.file_size = ntohll(requestBody.file_size);
	first = begin + sizeof(__int64);
	last = begin + 2 * sizeof(__int64);
	std::vector<int8_t> file_time_stamp(first, last);

	memcpy(&timeStamp.QuadPart, file_time_stamp.data(), file_time_stamp.size());
	timeStamp.QuadPart = ntohll(timeStamp.QuadPart);
	this->requestBody.file_timestamp.dwLowDateTime = timeStamp.LowPart;
	this->requestBody.file_timestamp.dwHighDateTime = timeStamp.HighPart;

	first = begin + 2 * sizeof(__int64);
	last = m_buffer.end();
	std::vector<int8_t> file_name(first, last);

	char name[256];
	memcpy(name, file_name.data(), file_name.size());
	name[file_name.size() - 2] = '\0'; // Needs to remove \r\n
	this->requestBody.file_name = std::string(name);

	return true;
}

/**
 * \brief this method prepare a protocol message and using all the information that has been passed with the constructor
 */
void ProtocolMessage::prepare_outgoing_packet() {

	switch(messageCode)
	{
	case MessageType::SEND:
		{
			prepare_send_message();
		}
		break;
	case MessageType::OK:
		{	
		
		}
		break;
	case MessageType::ERR:
		{
			// append the error and the error code 
			append(messageCode);
			append(errorCode);
		}
		break;
	default: 
		{
	
		}break;
	}
}

void ProtocolMessage::prepare_send_message(){

	append(protocol::MessageType::getMessageType(MessageType::SEND));
	append(static_cast<const char*>(messageBody.c_str()));

	__int64 fileSize = htonll(this->requestBody.file_size);
	append(reinterpret_cast<const char*>(&fileSize), sizeof(__int64));

	timeStamp.LowPart = this->requestBody.file_timestamp.dwLowDateTime;
	timeStamp.HighPart = this->requestBody.file_timestamp.dwHighDateTime;

	__int64 fileTimestamp = htonll(timeStamp.QuadPart);
	append(reinterpret_cast<const char*>(&fileTimestamp), sizeof(__int64));

	append(static_cast<const char*>(this->requestBody.file_name.c_str()), this->requestBody.file_name.size());

	append(static_cast<const char*>(endMessage.c_str()));
}

bool ProtocolMessage::get_packet_type() {
	char packet_type[5];
	
	try {
		if(m_buffer.empty())
		{
			messageCode = MessageType::UNDEFINED;
			return false;
		}

		memcpy(static_cast<void*>(packet_type), static_cast<void*>(&(*m_buffer.begin())), 4);
		packet_type[4] = '\0';

		// TODO check if the conversion goes wrong
		messageCode = protocol::MessageType::getMessageType(std::string(packet_type));
		return true;
	}
	catch (std::exception e) {
		messageCode = MessageType::UNDEFINED;
		return false;
	}
}

std::vector<int8_t> ProtocolMessage::get_packet_data() const {
	return m_buffer;
}


/*
 *					DISCOVERY MESSAGE IMPLEMENTATION			
 */

/*std::string discovery_message::get_packet_type() {

	const auto message = reinterpret_cast<char*>((m_buffer.data()));

	if (strlen(message) >= strlen(HELLO_MSG) && strncmp(message, HELLO_MSG, strlen(HELLO_MSG)) == 0)
		return HELLO_MSG;

	if (strlen(message) >= strlen(DISCOVERY_MSG) && strncmp(message, DISCOVERY_MSG, strlen(DISCOVERY_MSG)) == 0)
		return DISCOVERY_MSG;

	return nullptr;
}

std::string discovery_message::getUsername(char* username) {

	char packetType[] = HELLO_MSG;

	memcpy(static_cast<void*>(packetType), static_cast<void*>(&(*m_buffer.begin())), strlen(HELLO_MSG));

	if (packetType == HELLO_MSG)
		// HELLO_MSG is the smallest string within a discovery message packet
		memcpy(static_cast<void*>(username), static_cast<void*>(&(m_buffer.at(strlen(HELLO_MSG)))), strlen(HELLO_MSG));
	else
		throw message_exception("packet is not an Hello Message!\n");

	return nullptr;
}*/