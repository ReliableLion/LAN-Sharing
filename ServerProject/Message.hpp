#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <boost\asio.hpp>

class Msg_container {

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

	

protected:
	std::string messageBody = "";
	std::stringstream stream;
	const std::string endMessage = "\r\n";
	std::vector<boost::asio::const_buffer> bufferContainer;
};

class RequestMessage : public Msg_container {

public:
	RequestMessage() {};

	RequestMessage(__int64 fileSize, FILETIME fileTimestamp, std::string fileName) {

		Msg_container::messageBody.append(sendMessage);

		this->fileSize = fileSize;
		this->fileTimestamp = fileTimestamp;
		this->fileName = fileName;

		Msg_container::bufferContainer.push_back(boost::asio::buffer(reinterpret_cast<void*>(this->fileSize), sizeof(__int64)));

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

	const std::string getEndMessage() { return Msg_container::endMessage; }

private:
	const std::string sendMessage = "SEND ";
	__int64 fileSize;
	FILETIME fileTimestamp;
	std::string fileName;
	ULARGE_INTEGER timeStamp;
};

class DiscoveryMessage : public Msg_container {

public:
	DiscoveryMessage(std::string username) {
		Msg_container::messageBody.append(helloMsg);
		Msg_container::messageBody.append(username);
		Msg_container::messageBody.append(endMessage);

		bufferContainer.push_back(boost::asio::buffer(Msg_container::messageBody, Msg_container::messageBody.size()));
	}

	DiscoveryMessage() {
		Msg_container::messageBody.append(discoveryMsg);
		Msg_container::messageBody.append(endMessage);

		bufferContainer.push_back(boost::asio::buffer(Msg_container::messageBody, Msg_container::messageBody.size()));
	}

	const std::string helloMsg = "MYUSERNAME ";
	const std::string discoveryMsg = "LAN-SHARING LOOKINGFOR";

};

class ReplyMessage: public Msg_container {

};