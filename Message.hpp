#pragma once

#include "stdafx.h"

#include <sstream>
#include <vector>
#include <iostream>

#include "Protocol.hpp"
#include "Constants.hpp"
#include "Exceptions.hpp"


typedef struct {
    __int64 file_size;
    FILETIME file_timestamp;
    std::string file_name;
} request_struct;

class message {
public:

    message();

    message(const char *buffer, const int size);    // Will use existing allocated buffer and create packet from i
    // Packet(const Packet & p); 					// Will allocate new buffer but copy buffer from packet argument

    message(const protocol::message_code m);                     // Used for when sending a packet that only contains a packet type (Ex. End of File Packet)

    void append(const protocol::message_code mt);

    void append(const __int64 int64);

    void append(const std::size_t m);

    void append(const message &m);

    void append(const std::string &str);

    void append(const char *buffer, const int size);    // Will use existing allocated buffer and create packet from it

    void append(const protocol::error_code mt);

    void clear();

    std::vector<int8_t> m_buffer; // Message Packet Buffer

protected:
    std::string message_body_;
    std::stringstream stream_;
    const std::string end_message_ = "\r\n";
    size_t message_size_;
};

class RequestMessage : public message {
public:

    // This is used to create a RequestMessage, which it's supposed will be sent
    RequestMessage(__int64 fileSize, FILETIME fileTimestamp, std::string fileName);

    // This is used to create an empty RequestMessage, which it's supposed will be received
    RequestMessage() {};

    __int64 get_file_size() const {
        return this->requestBody.file_size;
    }

    FILETIME get_file_time_stamp() const {
        return this->requestBody.file_timestamp;
    }

    std::string get_file_name() const {
        return this->requestBody.file_name;
    }

    void prepare_message();

    request_struct get_request_data();

    void get_packet_type(char *packetType);

    std::string get_packet_data();

private:
    request_struct requestBody;
    ULARGE_INTEGER timeStamp;
};

class discovery_message : public message {
public:
    discovery_message(const std::string username) {
        append(HELLO_MSG);
        append(username);
        append(end_message_);
    }

    discovery_message() {};

    std::string get_username();

    std::string get_packet_type();

    std::string get_message_body();
};

class ProtocolMessage : public message {
    protocol::message_code message_code_;
    protocol::error_code error_code_;
    request_struct request_body_;
    ULARGE_INTEGER time_stamp_;

    int const min_size_request_ = (4 + (2 * sizeof(__int64)) + 1);
    int const max_size_request_ = (4 + (2 * sizeof(__int64)) + MAX_FILENAME_LENGTH_);

    void prepare_send_message();

public:
    ProtocolMessage(__int64 file_size, FILETIME file_timestamp, std::string file_name);     // This is used to create a RequestMessage, which it's supposed will be sent
    ProtocolMessage(protocol::message_code message_code);

    ProtocolMessage(protocol::error_code error);                                            // this constructor is used to build up an error message
    ProtocolMessage() {};                                                                   // This is used to create an empty RequestMessage, which it's supposed will be received


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

    std::vector<int8_t> get_packet_data() { return m_buffer; }

    request_struct get_message_request() { return request_body_; };

    protocol::message_code get_message_code() { return message_code_; };

    protocol::error_code get_error_code() { return error_code_; }
};