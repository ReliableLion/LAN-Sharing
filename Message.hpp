

#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>

class Message {

public:
	std::string getMessageBody() {
		return messageBody;
	}

protected:
	std::string messageBody = "";
	std::stringstream stream;
	const std::string endMessage = "\r\n";
	ULARGE_INTEGER timeStamp;
	std::string string;

};

class RequestMessage : public Message {

public:
	RequestMessage(__int64 fileSize, FILETIME fileTimestamp, std::string fileName) {

		messageBody.append(sendMessage);

		this->fileSize = fileSize;
		this->fileTimestamp = fileTimestamp;
		this->fileName = fileName;

		stream << this->fileSize;
		messageBody.append(stream.str());

		timeStamp.LowPart = this->fileTimestamp.dwLowDateTime;
		timeStamp.HighPart = this->fileTimestamp.dwHighDateTime;
		stream.clear();
		stream << timeStamp.QuadPart;

		messageBody.append(stream.str());

		messageBody.append(this->fileName);

		messageBody.append(endMessage);

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

};

class DiscoveryMessage: public Message {

public:
	DiscoveryMessage(std::string username) {
		messageBody.append(helloMsg);
		messageBody.append(username);
		messageBody.append(endMessage);
	}

	DiscoveryMessage() {
		messageBody.append(discoveryMsg);
		messageBody.append(endMessage);
	}

	const std::string helloMsg = "MYUSERNAME ";
	const std::string discoveryMsg = "LAN-SHARING LOOKINGFOR";

};