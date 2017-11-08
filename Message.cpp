#include "Message.hpp"
#include <cstdint> //Required to use std::int32_t
#include <WinSock2.h> //for htonl

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

		__int64 val = htonl((__int64)int64);
		Append((const char*)&val, sizeof(__int64));
	}

	void Message::Append(MessageType::TYPE mt) {

		Append((std::string)MessageType::getMessageType(mt));
	}

	void Message::Append(std::size_t s) {

		Append((__int64)s);
	}