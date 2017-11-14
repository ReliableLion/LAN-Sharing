#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <boost\asio.hpp>
#include "BadRequest.h"
#include ""

class Message {

public:
	std::vector<boost::asio::const_buffer> getMessageData() {
		return bufferContainer;
	}

	size_t getMessageSize() {
		return bufferContainer.size();
	}

	void setMessageData(std::vector<boost::asio::const_buffer> buffCont) {
		this->bufferContainer = buffCont;
	}

	const std::string getEndMessage() { return Message::endMessage; }
	

protected:
	std::string messageBody = "";
	std::stringstream stream;
	const std::string endMessage = "\r\n";
	std::vector<boost::asio::const_buffer> bufferContainer;
};

class RequestMessage : public Message {

public:
	RequestMessage() {};

	RequestMessage(__int64 fileSize, FILETIME fileTimestamp, std::string fileName) {

		Message::messageBody.append(sendMessage);

		this->fileSize = fileSize;
		this->fileTimestamp = fileTimestamp;
		this->fileName = fileName;

		Message::bufferContainer.push_back(boost::asio::buffer(reinterpret_cast<void*>(this->fileSize), sizeof(__int64)));

		timeStamp.LowPart = this->fileTimestamp.dwLowDateTime;
		timeStamp.HighPart = this->fileTimestamp.dwHighDateTime;
		bufferContainer.push_back(boost::asio::buffer(reinterpret_cast<void*>(this->timeStamp.QuadPart), sizeof(FILETIME)));

		messageBody.append(this->fileName);
		messageBody.append(endMessage);

		bufferContainer.push_back(boost::asio::buffer(messageBody, messageBody.size()));

	}

	void decodeRequest() {

	}

	__int64 getFileSize() {
		return this->fileSize;
	}

	FILETIME getFileTimeStamp() {
		return this->fileTimestamp;
	}

	std::string getFileName() {
		return this->fileName;
	}

private:
	const std::string sendMessage = "SEND ";
	__int64 fileSize;
	FILETIME fileTimestamp;
	std::string fileName;
	ULARGE_INTEGER timeStamp;
};

class DiscoveryMessage : public Message {

public:
	DiscoveryMessage(std::string username) {
		Message::messageBody.append(helloMsg);
		Message::messageBody.append(username);
		Message::messageBody.append(endMessage);

		bufferContainer.push_back(boost::asio::buffer(Message::messageBody, Message::messageBody.size()));
	}

	DiscoveryMessage() {
		Message::messageBody.append(discoveryMsg);
		Message::messageBody.append(endMessage);

		bufferContainer.push_back(boost::asio::buffer(Message::messageBody, Message::messageBody.size()));
	}

	const std::string helloMsg = "MYUSERNAME ";
	const std::string discoveryMsg = "LAN-SHARING LOOKINGFOR";

};

class ReplyMessage: public Message {
private:
	const std::string ok = "OK";
public:
	ReplyMessage() {};
	const std::string createReply() {
		Message::stream << ok << Message::endMessage;
		return stream.str();
	}

};