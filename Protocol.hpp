#pragma once
#include <map>

namespace protocol {
	class MessageType {

	public:

		static int protocol::MessageType::getMessageType(std::string msg_type);

		static std::string protocol::MessageType::getMessageType(int msg_type);

		const enum type {
			send,
			ok,
			err,
			undefined
		};

		static int get_error_type(std::string msg_type) {
			return 0;
		}

		const enum error_type {

		};

	private:
		static const std::map<std::string, int> message_type;
		const std::map<std::string, int> error_type_ = {};

	};
}