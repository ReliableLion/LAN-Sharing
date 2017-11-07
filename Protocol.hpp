#pragma once
#include <map>

namespace protocol {
	static class MessageType {

	public:

		int getMessageType(std::string msgType) {

			if (messageType.at(msgType) == SEND)
				return SEND;
			else if (messageType.at(msgType) == OK)
				return OK;
			else if (messageType.at(msgType) == ERROR)
				return ERROR;
			else
				return UNDEFINED;
		}

		const enum TYPE {
			SEND,
			OK,
			ERROR,
			UNDEFINED
		};

		int getErrorType(std::string msgType) {
			return 0;
		}

		const enum ERROR_TYPE {

		};

	private:
		const std::map<std::string, int> messageType = {
			std::make_pair(std::string("SEND"), 0), // Used when we want to send a file
			std::make_pair(std::string("+OK "), 1), // Used when the server accept the request
			std::make_pair(std::string("-ERR"), 2)  // Used to signal an error
		};
		const std::map<std::string, int> errorType = {};

	};
}