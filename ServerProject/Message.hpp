#pragma once
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <sstream>
#include "Protocol.hpp"
#include <vector>

using namespace protocol;

typedef struct {
	__int64 fileSize;
	FILETIME fileTimestamp;
	std::string fileName;
} requestStruct;

class Message {

public:

	Message();
	Message(const char * buffer, const int size); //Will use existing allocated buffer and create packet from it
												  //Packet(const Packet & p); //Will allocate new buffer but copy buffer from packet argument
	Message(const MessageType::TYPE m); //Used for when sending a packet that only contains a packet type (Ex. End of File Packet)
	void Append(const MessageType::TYPE mt);
	void Append(const __int64 int64);
	void Append(const std::size_t m);
	void Append(const Message & m);
	void Append(const std::string & str);
	void Append(const char * buffer, const int size); //Will use existing allocated buffer and create packet from it


	std::vector<int8_t> m_buffer; //Message Packet Buffer

	void Message::getPacketType(char* messageType);

protected:
	std::string messageBody = "";
	std::stringstream stream;
	const std::string endMessage = "\r\n";
	size_t messageSize;

};

class RequestMessage : public Message {

public:

	// This is used to create a RequestMessage, which it's supposed will be sent
	RequestMessage(__int64 fileSize, FILETIME fileTimestamp, std::string fileName);

	// This is used to create an empty RequestMessage, which it's supposed will be received
	RequestMessage() {};

	__int64 RequestMessage::getFileSize() {
		return this->requestBody.fileSize;
	}

	FILETIME RequestMessage::getFileTimeStamp() {
		return this->requestBody.fileTimestamp;
	}

	std::string RequestMessage::getFileName() {
		return this->requestBody.fileName;
	}

	void RequestMessage::prepareMessage();

	requestStruct RequestMessage::getRequestData();

private:
	requestStruct requestBody;
	ULARGE_INTEGER timeStamp;

};

class DiscoveryMessage : public Message {

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

	std::string getDiscoveryMessage() {
		return messageBody;
	}

};
