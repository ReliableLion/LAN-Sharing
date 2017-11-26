#include "Message.hpp"
#include "Exceptions.hpp"

#include <cstdint> //Required to use std::int32_t

message::message(): messageSize(0) {
}

message::message(const char * data_buffer, const int size) {

		Append(data_buffer, size);
	}

message::message(const MessageType::type pt) {

		Append(pt);
	}

void message::Append(const char * data_buffer, const int size) {

	m_buffer.insert(m_buffer.end(), data_buffer, data_buffer + size);
}

void message::Append(const std::string & str) {

	Append(str.c_str(), str.size());
}

void message::Append(const message & p) { //Allocate new block for buffer

	Append(reinterpret_cast<const char*>(&p.m_buffer), p.m_buffer.size());
}

void message::Append(const __int64 int64) {

	__int64 val = htonll(int64);
	Append(reinterpret_cast<const char*>(&val), sizeof(__int64));
}

void message::Append(const MessageType::type mt) {

	Append(static_cast<std::string>(MessageType::getMessageType(mt)));
}

void message::Append(const std::size_t s) {

	Append(static_cast<__int64>(s));
}

RequestMessage::RequestMessage(const __int64 file_size, const FILETIME file_timestamp, const std::string file_name) {

	messageBody.append(MessageType::getMessageType(MessageType::send));

	this->requestBody.file_size = file_size;
	this->requestBody.file_timestamp = file_timestamp;
	this->requestBody.file_name = file_name;
}

request_struct RequestMessage::get_request_data() {

	const std::vector<int8_t>::const_iterator begin = m_buffer.begin() + 4;

						/************************
						 *  GETTING FILE SIZE	*
						 ************************/
	std::vector<int8_t>::const_iterator first = begin;
	std::vector<int8_t>::const_iterator last = first + sizeof(__int64);
	std::vector<int8_t> file_size(first, last);

	memcpy(&requestBody.file_size, file_size.data(), file_size.size());
	requestBody.file_size = ntohll(requestBody.file_size);

						/****************************
						*  GETTING FILE TIME STAMP	*
						*****************************/
	first = begin + sizeof(__int64);
	last = begin + 2*sizeof(__int64);
	std::vector<int8_t> file_time_stamp(first, last);

	memcpy(&timeStamp.QuadPart, file_time_stamp.data(), file_time_stamp.size());
	timeStamp.QuadPart = ntohll(timeStamp.QuadPart);
	this->requestBody.file_timestamp.dwLowDateTime = timeStamp.LowPart;
	this->requestBody.file_timestamp.dwHighDateTime = timeStamp.HighPart;

						/************************
						*  GETTING FILE NAME	*
						*************************/
	first = begin + 2*sizeof(__int64);
	last = m_buffer.end();
	std::vector<int8_t> file_name(first, last);

	char name[256];
	memcpy(name, file_name.data(), file_name.size());
	name[file_name.size()-2] = '\0'; // Needs to remove \r\n
	this->requestBody.file_name = std::string(name);

	return this->requestBody;
}

void RequestMessage::prepare_message() {

	Append(static_cast<const char*>(messageBody.c_str()));

	__int64 fileSize = htonll(this->requestBody.file_size);
	Append(reinterpret_cast<const char*>(&fileSize), sizeof(__int64));

	timeStamp.LowPart = this->requestBody.file_timestamp.dwLowDateTime;
	timeStamp.HighPart = this->requestBody.file_timestamp.dwHighDateTime;

	__int64 fileTimestamp = htonll(timeStamp.QuadPart);
	Append(reinterpret_cast<const char*>(&fileTimestamp), sizeof(__int64));

	Append(static_cast<const char*>(this->requestBody.file_name.c_str()), this->requestBody.file_name.size());

	Append(static_cast<const char*>(endMessage.c_str()));
}

void RequestMessage::get_packet_type(char* packet_type) {

	try {
		memcpy(static_cast<void*>(packet_type), static_cast<void*>(&(*m_buffer.begin())), 4);
		packet_type[4] = '\0';
	} catch(std::exception e) {
		throw std::exception("Exception during packet type getting!");
	}
}

std::vector<int8_t> RequestMessage::get_packet_data() const {
	return m_buffer;
}

std::string discovery_message::get_packet_type() {

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

	if (packetType == HELLO_MSG) {
		// HELLO_MSG is the smallest string within a discovery message packet
		memcpy(static_cast<void*>(username), static_cast<void*>(&(m_buffer.at(strlen(HELLO_MSG)))), m_buffer.size() - strlen(HELLO_MSG));
		username[m_buffer.size() - strlen(HELLO_MSG)] = '\0';
	}
	else
		throw message_exception("packet is not an Hello Message!\n");

	return nullptr;
}

std::string discovery_message::get_message_body() {
	return std::string(reinterpret_cast<char*>(m_buffer.data()));
}
