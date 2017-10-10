

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

		stream << fileSize;
		messageBody.append(stream.str());

		timeStamp.LowPart = fileTimestamp.dwLowDateTime;
		timeStamp.HighPart = fileTimestamp.dwHighDateTime;
		stream.clear();
		stream << timeStamp.QuadPart;

		messageBody.append(stream.str());

		messageBody.append(fileName);

		messageBody.append(endMessage);

	}

private:
	const std::string sendMessage = "SEND ";

};

class DiscoveryMessage: public Message {

public:
	DiscoveryMessage(std::string username) {
		messageBody.append(discoveryMsg);
		messageBody.append(username);
		messageBody.append(endMessage);
	}

private:
	const std::string discoveryMsg = "MYUSERNAME ";

};