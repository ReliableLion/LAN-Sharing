#include "Message.hpp"
#include "Exceptions.hpp"
#include <cstdint> //Required to use std::int32_t
#include <iostream>

message::message(): message_size_(0) {
}

message::message(const char * data_buffer, const int size) {

		Append(data_buffer, size);
	}

void message::clear(){
	m_buffer.clear();
}

message::message(const MessageType::message_code pt) {

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

void message::Append(const MessageType::message_code mt) {

	Append(static_cast<std::string>(MessageType::get_message_type(mt)));
}

void message::Append(const MessageType::error_code mt) {
	std::string res_code_msg = MessageType::get_error_type(mt);

	if (res_code_msg.empty())
		Append(static_cast<std::string>(res_code_msg));

	// TODO check if should be raised an exception
}

void message::Append(const std::size_t s) {

	Append(static_cast<__int64>(s));
}

RequestMessage::RequestMessage(const __int64 file_size, const FILETIME file_timestamp, const std::string file_name) {

	message_body_.append(MessageType::get_message_type(MessageType::SEND));

	this->requestBody.file_size = file_size;
	this->requestBody.file_timestamp = file_timestamp;
	this->requestBody.file_name = file_name;
}

request_struct RequestMessage::get_request_data() {

	const std::vector<int8_t>::const_iterator begin = m_buffer.begin() + 4;

						/************************
						 *  GETTING FILE SIZE	*
						 ************************/
	auto first = begin;
	auto last = first + sizeof(__int64);
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

	Append(static_cast<const char*>(message_body_.c_str()));

	__int64 fileSize = htonll(this->requestBody.file_size);
	Append(reinterpret_cast<const char*>(&fileSize), sizeof(__int64));

	timeStamp.LowPart = this->requestBody.file_timestamp.dwLowDateTime;
	timeStamp.HighPart = this->requestBody.file_timestamp.dwHighDateTime;

	__int64 fileTimestamp = htonll(timeStamp.QuadPart);
	Append(reinterpret_cast<const char*>(&fileTimestamp), sizeof(__int64));

	Append(static_cast<const char*>(this->requestBody.file_name.c_str()), this->requestBody.file_name.size());

	Append(static_cast<const char*>(end_message_.c_str()));
}

void RequestMessage::get_packet_type(char* packet_type) {

	try {
		memcpy(static_cast<void*>(packet_type), static_cast<void*>(&(*m_buffer.begin())), 4);
		packet_type[4] = '\0';
	} catch(std::exception e) {
		throw std::exception("Exception during packet type getting!");
	}
}

std::string RequestMessage::get_packet_data() {
	const auto temp = reinterpret_cast<char*>((m_buffer.data()));
	return std::string(temp, m_buffer.size());
}

discovery_message::discovery_message() : message() {
}
std::string discovery_message::get_packet_type() {

	const auto temp = reinterpret_cast<char*>((m_buffer.data()));
	auto message = std::string(temp, m_buffer.size());

	if (strlen(message.c_str()) >= strlen(HELLO_MSG) && strncmp(message.c_str(), HELLO_MSG, strlen(HELLO_MSG)) == 0)
		return HELLO_MSG;
	
	if (strlen(message.c_str()) >= strlen(DISCOVERY_MSG) && strncmp(message.c_str(), DISCOVERY_MSG, strlen(DISCOVERY_MSG)) == 0)
		return DISCOVERY_MSG;
	
	return "";
}

std::string discovery_message::get_username() {
	
	char username[USERNAME_LENGTH] = "";
	
	if (get_packet_type() == HELLO_MSG) {
		// HELLO_MSG is the smallest string within a discovery message packet
		memcpy(static_cast<void*>(username), static_cast<void*>(&(m_buffer.at(strlen(HELLO_MSG)))), m_buffer.size() - strlen(HELLO_MSG));
		username[m_buffer.size() - strlen(HELLO_MSG)] = '\0';
	}
	else
		throw message_exception("packet is not an Hello Message!\n");

	const auto temp = reinterpret_cast<char*>(username);
	return std::string(temp, strlen(username));
}

std::string discovery_message::get_message_body() {
	const auto temp = reinterpret_cast<char*>((m_buffer.data()));
	return std::string(temp, m_buffer.size());
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
	this->message_code_ = MessageType::SEND;

	// insert the request information
	this->request_body_.file_size = file_size;
	this->request_body_.file_timestamp = file_timestamp;
	this->request_body_.file_name = file_name;
}

ProtocolMessage::ProtocolMessage(protocol::MessageType::message_code message_code) {
	this->message_code_ = message_code;
}

ProtocolMessage::ProtocolMessage(protocol::MessageType::error_code error) {
	this->message_code_ = MessageType::ERR;
	this->error_code_ = error;
}

/**
* \brief
* \return
*/
bool ProtocolMessage::convert_incoming_packet() {
	// TODO check if the buffer contain the exact size of data
	if (m_buffer.size() < min_size_request_ ||
		m_buffer.size() > max_size_request_) {
		return false;
	}

	const std::vector<int8_t>::const_iterator begin = m_buffer.begin() + 4;

	std::vector<int8_t>::const_iterator first = begin;
	std::vector<int8_t>::const_iterator last = first + sizeof(__int64);
	std::vector<int8_t> file_size(first, last);

	memcpy(&request_body_.file_size, file_size.data(), file_size.size());
	request_body_.file_size = ntohll(request_body_.file_size);
	first = begin + sizeof(__int64);
	last = begin + 2 * sizeof(__int64);
	std::vector<int8_t> file_time_stamp(first, last);

	memcpy(&time_stamp_.QuadPart, file_time_stamp.data(), file_time_stamp.size());
	time_stamp_.QuadPart = ntohll(time_stamp_.QuadPart);
	this->request_body_.file_timestamp.dwLowDateTime = time_stamp_.LowPart;
	this->request_body_.file_timestamp.dwHighDateTime = time_stamp_.HighPart;

	first = begin + 2 * sizeof(__int64);
	last = m_buffer.end();
	std::vector<int8_t> file_name(first, last);

	char name[256];
	memcpy(name, file_name.data(), file_name.size());
	name[file_name.size() - 2] = '\0'; // Needs to remove \r\n
	this->request_body_.file_name = std::string(name);

	return true;
}

/**
* \brief this method prepare a protocol message and using all the information that has been passed with the constructor
*/
void ProtocolMessage::prepare_outgoing_packet() {
	switch (message_code_) {
	case MessageType::SEND:
	{
		Append(protocol::MessageType::SEND);
		prepare_send_message();
	}
	break;
	case MessageType::OK:
	{
		Append(protocol::MessageType::OK);
	}
	break;
	case MessageType::ERR:
	{
		// append the error and the error code 
		Append(protocol::MessageType::ERR);
		Append(error_code_);
	}
	break;
	default:
		break;
	}
}

void ProtocolMessage::prepare_send_message() {
	Append(static_cast<const char*>(message_body_.c_str()));

	__int64 fileSize = htonll(this->request_body_.file_size);
	Append(reinterpret_cast<const char*>(&fileSize), sizeof(__int64));

	time_stamp_.LowPart = this->request_body_.file_timestamp.dwLowDateTime;
	time_stamp_.HighPart = this->request_body_.file_timestamp.dwHighDateTime;

	__int64 fileTimestamp = htonll(time_stamp_.QuadPart);
	Append(reinterpret_cast<const char*>(&fileTimestamp), sizeof(__int64));

	Append(static_cast<const char*>(this->request_body_.file_name.c_str()), this->request_body_.file_name.size());

	Append(static_cast<const char*>(end_message_.c_str()));
}

bool ProtocolMessage::get_packet_type() {
	char packet_type[5];

	try {
		if (m_buffer.empty()) {
			message_code_ = MessageType::UNDEFINED;
			return false;
		}

		memcpy(static_cast<void*>(packet_type), static_cast<void*>(&(*m_buffer.begin())), 4);
		packet_type[4] = '\0';

		// TODO check if the conversion goes wrong
		message_code_ = protocol::MessageType::get_message_type(std::string(packet_type));
		return true;
	}
	catch (std::exception e) {
		message_code_ = MessageType::UNDEFINED;
		return false;
	}
}

std::vector<int8_t> ProtocolMessage::get_packet_data() const {
	return m_buffer;
}