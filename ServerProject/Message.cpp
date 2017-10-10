#include "Message.h"

std::string ResponseMsg::sendResponse() {
	
	// flushing the stream associated witht the message
	Message::stream.flush();
	stream << ok_msg << Message::end_message;
	return stream.str;

}