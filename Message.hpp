#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

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

	std::vector<boost::asio::const_buffer> bufferContainer;

protected:
	std::string messageBody = "";
	std::stringstream stream;
	const std::string endMessage = "\r\n";

};

class RequestMessage : public Message {

public:
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

	}

	RequestMessage() {

		requestBuffer = {
			boost::asio::buffer(&this->requestBody.fileSize, sizeof(this->requestBody.fileSize)),
			boost::asio::buffer(&this->timeStamp.QuadPart, sizeof(this->requestBody.fileTimestamp)),
			boost::asio::buffer(&this->requestBody.fileName, 256) };
	}

	__int64 getFileSize() {
		return this->requestBody.fileSize;
	}

	FILETIME getFileTimeStamp() {
		return this->requestBody.fileTimestamp;
	}

	std::string getFileName() {
		return this->requestBody.fileName;
	}

	boost::array<boost::asio::mutable_buffer, 3> requestBuffer;

private:
	const std::string sendMessage = "SEND ";
	typedef struct {
		__int64 fileSize;
		FILETIME fileTimestamp;
		std::string fileName;
	} requestStruct;
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

};