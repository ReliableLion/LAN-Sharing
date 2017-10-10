#ifndef MESSAGE_H 
#define	MESSAGE_H

#include <string>
#include <sstream>

typedef enum {
	refuse_conn,			// error 1: server refuse the connection with the client
	conn_closed				// error 2: connection closed
} error_codes;


class Message {
protected:
	std::string message;
	std::stringstream stream;
	const std::string end_message = "/r/n";

public:

	std::string getMessageBody() { return this->message; }


};


class ResponseMsg: public  Message {
private:
	const std::string ok_msg = "ok";
public:
	ResponseMsg() = default;
	std::string sendResponse();

};

class ErrorMsg : public Message {
private: 
	error_codes err;

public: 
	ErrorMsg() = default;
	std::string getErrorCode();

};

#endif // !MESSAGE_H 
