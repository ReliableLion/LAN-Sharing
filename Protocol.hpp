#pragma once

#include <map>

namespace protocol {

    // enumerator declaration
    enum message_code {
        send,                        // code used to send the request to the server
        ok,                            // code to responde affermatively to the request
        err,                        // code that specify a error
        undefined                    // message code not recognized
    };

    enum error_code {
        err_1
    };

    class MessageType {
    private:
        static const std::map<std::string, int> MESSAGE_TYPE_MAP;
        static const std::map<std::string, int> ERROR_TYPE_MAP;
    public:
        static message_code get_message_type(std::string msg_type);

        static std::string get_message_type(protocol::message_code msg_type);

        static error_code get_error_type(std::string error_code);

        static std::string get_error_type(protocol::error_code error_code);
    };

} // end of namespace protocol 
