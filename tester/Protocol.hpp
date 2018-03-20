#pragma once

#include <map>

namespace protocol {

	class MessageType {
		static const std::map<std::string, int> messageType;
		static const std::map<std::string, int> errorType;

	public:
		// enumerator declaration
		const enum message_code {
			SEND,
			OK,
			ERR,
			UNDEFINED
		};

		const enum error_code {
			ERR_1
		};

		static protocol::MessageType::message_code getMessageType(std::string msgType);
		static std::string getMessageType(protocol::MessageType::message_code msgType);

		static protocol::MessageType::error_code getErrorType(std::string errorCode);
		static std::string getErrorType(protocol::MessageType::error_code errorCode);
	};

} // end of namespace protocol 
