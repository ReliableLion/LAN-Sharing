#pragma once
#include <map>

namespace protocol {
	class MessageType {

	public:

		static int protocol::MessageType::getMessageType(std::string msgType);

		static std::string protocol::MessageType::getMessageType(int msgType);

		const enum TYPE {
			SEND,
			OK,
			ERR,
			UNDEFINED
		};

		int getErrorType(std::string msgType) {
			return 0;
		}

		const enum ERROR_TYPE {

		};

	private:
		static const std::map<std::string, int> messageType;
		const std::map<std::string, int> errorType = {};

	};
}