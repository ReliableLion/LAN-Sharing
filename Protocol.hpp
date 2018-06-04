#pragma once

#include <map>

namespace protocol {

    // enumerator declaration
    enum message_code {
        SEND,                        // code used to send the request to the server
        OK,                            // code to responde affermatively to the request
        ERR,                        // code that specify a error
        UNDEFINED                    // message code not recognized
    };

    enum error_code {
        ERR_1
    };

    class MessageType {
    private:
        static const std::map<std::string, int> MESSAGE_TYPE_MAP;
        static const std::map<std::string, int> ERROR_TYPE_MAP;
    public:
        static message_code get_message_type(std::string msgType);

        static std::string get_message_type(protocol::message_code msgType);

        static error_code get_error_type(std::string errorCode);

        static std::string get_error_type(protocol::error_code errorCode);
    };

} // end of namespace protocol 
