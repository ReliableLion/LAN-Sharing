#pragma once

#include <map>

namespace protocol {

	class MessageType {
		static const std::map<std::string, int> message_type;
		static const std::map<std::string, int> error_type;

	public:
		// enumerator declaration
		const enum message_code {
			SEND,						// code used to send the request to the server
			OK,							// code to responde affermatively to the request
			ERR,						// code that specify a error 
			UNDEFINED					// message code not recognized
		};

		const enum error_code {
			ERR_1
		};

		static message_code get_message_type(std::string msgType);
		static std::string get_message_type(protocol::MessageType::message_code msgType);

		static error_code get_error_type(std::string errorCode);
		static std::string get_error_type(protocol::MessageType::error_code errorCode);
	};

} // end of namespace protocol 
