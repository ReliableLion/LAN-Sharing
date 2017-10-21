#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>

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

		this->fileSize = fileSize;
		this->fileTimestamp = fileTimestamp;
		this->fileName = fileName;

		bufferContainer.push_back(boost::asio::buffer(reinterpret_cast<void*>(this->fileSize), sizeof(__int64)));

		timeStamp.LowPart = this->fileTimestamp.dwLowDateTime;
		timeStamp.HighPart = this->fileTimestamp.dwHighDateTime;
		bufferContainer.push_back(boost::asio::buffer(reinterpret_cast<void*>(this->timeStamp.QuadPart), sizeof(FILETIME)));

		messageBody.append(this->fileName);
		messageBody.append(endMessage);

		bufferContainer.push_back(boost::asio::buffer(messageBody, messageBody.size()));

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