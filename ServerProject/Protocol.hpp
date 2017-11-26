#pragma once
#include <map>

namespace protocol {

class MessageType {
public:
	static int protocol::MessageType::getMessageType(std::string msgType);
	static std::string protocol::MessageType::getMessageType(int msgType);
	int getErrorType(std::string msgType) { return 0; }

public: // enumerator declaration
	const enum TYPE {
		send,
		ok,
		err,
		undefined
	};

	const enum ERROR_TYPE {

	};

private:
	static const std::map<std::string, int> messageType;
	static const std::map<std::string, int> errorType;
};

} // end of namespace protocol 
