#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

typedef struct {
	__int64 fileSize;
	FILETIME fileTimestamp;
	std::string fileName;
} requestStruct;

class Message {

public:

	size_t getMessageSize() {
		return messageSize;
	}

protected:
	std::string messageBody = "";
	std::stringstream stream;
	const std::string endMessage = "\r\n";
	size_t messageSize;

};

class RequestMessage : public Message {

public:

	// This is used to create a RequestMessage, which it's supposed will be sent
	RequestMessage(__int64 fileSize, FILETIME fileTimestamp, std::string fileName) {

		messageBody.append(sendMessage);

		this->requestBody.fileSize = fileSize;
		this->requestBody.fileTimestamp = fileTimestamp;
		this->requestBody.fileName = fileName;

		bufferContainer.push_back(boost::asio::buffer(&this->requestBody.fileSize, sizeof(this->requestBody.fileSize)));

		timeStamp.LowPart = this->requestBody.fileTimestamp.dwLowDateTime;
		timeStamp.HighPart = this->requestBody.fileTimestamp.dwHighDateTime;
		bufferContainer.push_back(boost::asio::buffer(&this->timeStamp.QuadPart, sizeof(this->requestBody.fileTimestamp)));

		messageBody.append(this->requestBody.fileName);
		messageBody.append(endMessage);

		bufferContainer.push_back(boost::asio::buffer(messageBody, messageBody.size()));

		// Set the size of the request message
		messageSize = bufferContainer.size();

	}

	// This is used to create an empty RequestMessage, which it's supposed will be received
	RequestMessage() {

		requestBuffer = {
			boost::asio::buffer(&this->requestBody.fileSize, sizeof(this->requestBody.fileSize)),
			boost::asio::buffer(&this->timeStamp.QuadPart, sizeof(this->requestBody.fileTimestamp)),
			boost::asio::buffer(&this->requestBody.fileName, 256) };
	}

	__int64 RequestMessage::getFileSize() {
		return this->requestBody.fileSize;
	}

	FILETIME RequestMessage::getFileTimeStamp() {
		return this->requestBody.fileTimestamp;
	}

	std::string RequestMessage::getFileName() {
		return this->requestBody.fileName;
	}

	RequestMessage::requestStruct RequestMessage::getRequestData() {
		boost::asio::mutable_buffer tempBuffer;
		std::size_t s1;
		unsigned char* p1;

		tempBuffer = requestBuffer[0];
		s1 = boost::asio::buffer_size(tempBuffer);
		p1 = boost::asio::buffer_cast<unsigned char*>(tempBuffer);
		this->requestBody.fileSize = reinterpret_cast<__int64>(p1);

		tempBuffer = requestBuffer[1];
		s1 = boost::asio::buffer_size(tempBuffer);
		p1 = boost::asio::buffer_cast<unsigned char*>(tempBuffer);
		timeStamp.QuadPart = reinterpret_cast<__int64>(p1);
		this->requestBody.fileTimestamp.dwLowDateTime = timeStamp.LowPart;
		this->requestBody.fileTimestamp.dwHighDateTime = timeStamp.HighPart;

		tempBuffer = requestBuffer[2];
		s1 = boost::asio::buffer_size(tempBuffer);
		p1 = boost::asio::buffer_cast<unsigned char*>(tempBuffer);
		this->requestBody.fileName = std::string(reinterpret_cast<char*>(p1));

		return this->requestBody;
	}

	boost::array<boost::asio::mutable_buffer, 3> requestBuffer;
	std::vector<boost::asio::const_buffer> bufferContainer;

private:
	const std::string sendMessage = "SEND ";
	requestStruct requestBody;
	ULARGE_INTEGER timeStamp;

};

class DiscoveryMessage: public Message {

public:
	DiscoveryMessage(std::string username) {
		messageBody.append(helloMsg);
		messageBody.append(username);
		messageBody.append(endMessage);

		bufferContainer.push_back(boost::asio::buffer(messageBody, messageBody.size()));
	}

	DiscoveryMessage() {
		messageBody.append(discoveryMsg);
		messageBody.append(endMessage);

		bufferContainer.push_back(boost::asio::buffer(messageBody, messageBody.size()));
	}

	const std::string helloMsg = "MYUSERNAME ";
	const std::string discoveryMsg = "LAN-SHARING LOOKINGFOR";

	std::string getDiscoveryMessage() {
		return messageBody;
	}

private:
	std::vector<boost::asio::const_buffer> bufferContainer;


};