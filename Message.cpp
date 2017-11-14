#include "Message.hpp"
#include <cstdint> //Required to use std::int32_t

Message::Message(){}

Message::Message(const char * dataBuffer, const int size) {

		Append(dataBuffer, size);
	}

Message::Message(MessageType::TYPE pt) {

		Append(pt);
	}

void Message::Append(const char * dataBuffer, const int size) {

	m_buffer.insert(m_buffer.end(), dataBuffer, dataBuffer + size);
}

void Message::Append(const std::string & str) {

	Append(str.c_str(), str.size());
}

void Message::Append(const Message & p) { //Allocate new block for buffer

	Append((const char*)&p.m_buffer, p.m_buffer.size());
}

void Message::Append(__int64 int64) {

	__int64 val = htonll(int64);
	Append((const char*)&val, sizeof(__int64));
}

void Message::Append(MessageType::TYPE mt) {

	Append((std::string)MessageType::getMessageType(mt));
}

void Message::Append(std::size_t s) {

	Append((__int64)s);
}

RequestMessage::RequestMessage(__int64 fileSize, FILETIME fileTimestamp, std::string fileName) {

	messageBody.append(MessageType::getMessageType(MessageType::SEND));

	this->requestBody.fileSize = fileSize;
	this->requestBody.fileTimestamp = fileTimestamp;
	this->requestBody.fileName = fileName;
}

requestStruct RequestMessage::getRequestData() {

	std::vector<int8_t>::const_iterator first;
	std::vector<int8_t>::const_iterator last;

	first = m_buffer.begin();
	last = m_buffer.begin() + sizeof(__int64);
	std::vector<int8_t> fileSize(first, last);

	memcpy((void *)this->requestBody.fileSize, (void*)&(*m_buffer.begin()), sizeof(__int64));

	first = m_buffer.begin() + sizeof(__int64);
	last = m_buffer.begin() + sizeof(__int64) + sizeof(__int64);
	std::vector<int8_t> fileTimeStamp(first, last);

	memcpy((void *)timeStamp.QuadPart, (void*)&(*(m_buffer.begin() + sizeof(__int64))), sizeof(__int64));
	this->requestBody.fileTimestamp.dwLowDateTime = timeStamp.LowPart;
	this->requestBody.fileTimestamp.dwHighDateTime = timeStamp.HighPart;

	first = m_buffer.begin() + sizeof(__int64) + sizeof(__int64);
	last = m_buffer.end();
	std::vector<int8_t> fileName(first, last);

	char name[256];
	memcpy((void *)name, (void*)&(*(m_buffer.begin() + sizeof(__int64) + sizeof(__int64))), last - first);
	this->requestBody.fileName = std::string(name);

	return this->requestBody;
}

void RequestMessage::prepareMessage() {

	Message::Append((const char*)messageBody.c_str());

	__int64 fileSize = htonll(this->requestBody.fileSize);
	Message::Append((const char*)&fileSize, sizeof(__int64));

	timeStamp.LowPart = this->requestBody.fileTimestamp.dwLowDateTime;
	timeStamp.HighPart = this->requestBody.fileTimestamp.dwHighDateTime;

	__int64 fileTimestamp = htonll(timeStamp.QuadPart);
	Message::Append((const char*)&fileTimestamp, sizeof(__int64));

	Message::Append((const char*)this->requestBody.fileName.c_str(), sizeof(this->requestBody.fileName.size()));

	Message::Append((const char*)endMessage.c_str());
}