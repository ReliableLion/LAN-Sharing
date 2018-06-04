#pragma once

#include "stdafx.h"

#include <sstream>
#include <vector>

#include "Protocol.hpp"
#include "Constants.hpp"


typedef struct {
    __int64 file_size_;
    FILETIME file_timestamp_;
    std::string file_name_;
} request_struct;

class Message {
public:

    Message();

    Message(const char *buffer, const int size);    // Will use existing allocated buffer and create packet from i
    // Packet(const Packet & p); 					// Will allocate new buffer but copy buffer from packet argument

	explicit Message(const protocol::message_code m);                     // Used for when sending a packet that only contains a packet type (Ex. End of File Packet)

    void append(const protocol::message_code mt);

    void append(const __int64 int64);

    void append(const std::size_t m);

    void append(const Message &m);

    void append(const std::string &str);

    void append(const char *buffer, const int size);    // Will use existing allocated buffer and create packet from it

    void append(const protocol::error_code mt);

    void clear();

    std::vector<int8_t> m_buffer_; // Message Packet Buffer

protected:
    std::string message_body_;
    std::stringstream stream_;
    const std::string END_MESSAGE_ = "\r\n";
    size_t message_size_;
};

class RequestMessage : public Message {
public:

    // This is used to create a RequestMessage, which it's supposed will be sent
    RequestMessage(__int64 file_size, FILETIME file_timestamp, std::string file_name);

    // This is used to create an empty RequestMessage, which it's supposed will be received
    RequestMessage() {};

    __int64 get_file_size() const {
        return this->request_body_.file_size_;
    }

    FILETIME get_file_time_stamp() const {
        return this->request_body_.file_timestamp_;
    }

    std::string get_file_name() const {
        return this->request_body_.file_name_;
    }

    void prepare_message();

    request_struct get_request_data();

    void get_packet_type(char *packet_type);

    std::string get_packet_data();

private:
    request_struct request_body_;
    ULARGE_INTEGER time_stamp_;
};

class DiscoveryMessage : public Message {
public:
	explicit DiscoveryMessage(const std::string username) {
        append(HELLO_MSG);
        append(username);
        append(END_MESSAGE_);
    }

    DiscoveryMessage() {};

    std::string get_username();

    std::string get_packet_type();

    std::string get_message_body();
};

class ProtocolMessage : public Message {
    protocol::message_code message_code_;
    protocol::error_code error_code_;
    request_struct request_body_;
    ULARGE_INTEGER time_stamp_;

    int const MIN_SIZE_REQUEST_ = (4 + (2 * sizeof(__int64)) + 1);
    int const MAX_SIZE_REQUEST_ = (4 + (2 * sizeof(__int64)) + MAX_FILENAME_LENGTH);

    void prepare_send_message();

public:
    ProtocolMessage(__int64 file_size, FILETIME file_timestamp, std::string file_name);     // This is used to create a RequestMessage, which it's supposed will be sent
	explicit ProtocolMessage(protocol::message_code message_code);

	explicit ProtocolMessage(protocol::error_code error);                                            // this constructor is used to build up an error message
    ProtocolMessage(): message_code_(), error_code_() {
	} ;                                                                   // This is used to create an empty RequestMessage, which it's supposed will be received


    /*__int64 ProtocolMessage::get_file_size() const {
    return this->requestBody.file_size;
}

    std::string ProtocolMessage::get_file_name() const {
     this->requestBody.file_name;
    }*/

    // this method is used to send out a packet
    void prepare_out_packet();

    // this two methods are used to decode a single packet
    void compute_packet_type();

    bool compute_request();

    std::vector<int8_t> get_packet_data() const { return m_buffer_; }

    request_struct get_message_request() const { return request_body_; };

    protocol::message_code get_message_code() const { return message_code_; };

    protocol::error_code get_error_code() const { return error_code_; }
};