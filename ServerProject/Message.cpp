#include "Message.h"

std::string ResponseMsg::sendResponse() {
	
	// flushing the stream associated witht the message
	Message::stream.flush();
	stream << ok_msg << Message::end_message;
	message = stream.str;
	return getMessageBody();
}

std::string ErrorMsg::getErrorCode(error_codes err) {

	// flushing the stream associated with the message
	Message::stream.flush();
	stream << error_msg << err << Message::end_message;
	message = stream.str;
	return getMessageBody();
}