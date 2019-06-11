#pragma once

#include <map>
#include <string>

namespace protocol {

    // enumerator declaration
    enum message_code {
        send,                        // code used to send the request to the server
        ok,                            // code to responde affermatively to the request
        err,                        // code that specify a error
        undefined                    // message code not recognized
    };

    enum error_code {
        err_1, // Generic error
		err_file,
		err_timeout,
		err_packet_format
    };

    class MessageType {
    private:
        static std::map<std::string, message_code> MESSAGE_TYPE_MAP;
        static std::map<std::string, error_code> ERROR_TYPE_MAP;
    public:
        static message_code get_message_type(std::string msg_type);

        static std::string get_message_type(protocol::message_code msg_type);

        static error_code get_error_type(std::string error_code);

        static std::string get_error_type(protocol::error_code error_code);
    };

} // end of namespace protocol 
