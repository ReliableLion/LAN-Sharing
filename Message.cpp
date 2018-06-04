#include "stdafx.h"
#include "Message.hpp"
#include "Exceptions.hpp"

Message::Message() : message_body_(""), message_size_(0) {}

Message::Message(const char *data_buffer, const int size) { append(data_buffer, size); }

Message::Message(const protocol::message_code pt) { append(pt); }

void Message::append(const char *data_buffer, const int size) {
    m_buffer_.insert(m_buffer_.end(), data_buffer, data_buffer + size);
}

void Message::append(const std::string &str) {
    append(str.c_str(), str.size());
}

void Message::append(const Message &p) {
    //Allocate new block for buffer
    append(reinterpret_cast<const char *>(&p.m_buffer_), p.m_buffer_.size());
}

void Message::append(const __int64 int64) {
    __int64 val = htonll(int64);
    append(reinterpret_cast<const char *>(&val), sizeof(__int64));
}

void Message::append(const protocol::message_code mt) {
    append(static_cast<std::string>(protocol::MessageType::get_message_type(mt)));
}

void Message::append(const protocol::error_code mt) {
	auto res_code_msg = protocol::MessageType::get_error_type(mt);

    if (res_code_msg.empty())
        append(static_cast<std::string>(res_code_msg));

    // TODO check if should be raised an exception
}

void Message::append(const std::size_t s) {
    append(static_cast<__int64>(s));
}

void Message::clear() { m_buffer_.clear(); }

/********************/
/* 	REQUEST MESSAGE */
/********************/


RequestMessage::RequestMessage(const __int64 file_size, const FILETIME file_timestamp, const std::string file_name) {

    message_body_.append(protocol::MessageType::get_message_type(protocol::send));

    this->request_body_.file_size_ = file_size;
    this->request_body_.file_timestamp_ = file_timestamp;
    this->request_body_.file_name_ = file_name;
}

request_struct RequestMessage::get_request_data() {

    const std::vector<int8_t>::const_iterator begin = m_buffer_.begin() + 4;

    /************************
     *  GETTING FILE SIZE	*
     ************************/
    auto first = begin;
    auto last = first + sizeof(__int64);
    std::vector<int8_t> file_size(first, last);

    memcpy(&request_body_.file_size_, file_size.data(), file_size.size());
    request_body_.file_size_ = ntohll(request_body_.file_size_);

    /****************************
    *  GETTING FILE TIME STAMP	*
    *****************************/
    first = begin + sizeof(__int64);
    last = begin + 2 * sizeof(__int64);
    std::vector<int8_t> file_time_stamp(first, last);

    memcpy(&time_stamp_.QuadPart, file_time_stamp.data(), file_time_stamp.size());
    time_stamp_.QuadPart = ntohll(time_stamp_.QuadPart);
    this->request_body_.file_timestamp_.dwLowDateTime = time_stamp_.LowPart;
    this->request_body_.file_timestamp_.dwHighDateTime = time_stamp_.HighPart;

    /************************
    *  GETTING FILE NAME	*
    *************************/
    first = begin + 2 * sizeof(__int64);
    last = m_buffer_.end();
    std::vector<int8_t> file_name(first, last);

    char name[256];
    memcpy(name, file_name.data(), file_name.size());
    name[file_name.size() - 2] = '\0'; // Needs to remove \r\n
    this->request_body_.file_name_ = std::string(name);

    return this->request_body_;
}

void RequestMessage::prepare_message() {

    append(static_cast<const char *>(message_body_.c_str()));

    __int64 file_size = htonll(this->request_body_.file_size_);
    append(reinterpret_cast<const char *>(&file_size), sizeof(__int64));

    time_stamp_.LowPart = this->request_body_.file_timestamp_.dwLowDateTime;
    time_stamp_.HighPart = this->request_body_.file_timestamp_.dwHighDateTime;

    __int64 file_timestamp = htonll(time_stamp_.QuadPart);
    append(reinterpret_cast<const char *>(&file_timestamp), sizeof(__int64));

    append(static_cast<const char *>(this->request_body_.file_name_.c_str()), this->request_body_.file_name_.size());

    append(static_cast<const char *>(END_MESSAGE_.c_str()));
}

void RequestMessage::get_packet_type(char *packet_type) {

    try {
        memcpy(static_cast<void *>(packet_type), static_cast<void *>(&(*m_buffer_.begin())), 4);
        packet_type[4] = '\0';
    } catch (std::exception &e) {
        throw std::exception("Exception during packet type getting!");
    }

}

std::string RequestMessage::get_packet_data() {
    const auto temp = reinterpret_cast<char *>((m_buffer_.data()));
    return std::string(temp, m_buffer_.size());
}



/**********************/
/* 	DISCOVERY MESSAGE */
/**********************/



std::string DiscoveryMessage::get_packet_type() {

    const auto temp = reinterpret_cast<char *>((m_buffer_.data()));
    auto message = std::string(temp, m_buffer_.size());

    if (strlen(message.c_str()) >= strlen(HELLO_MSG) && strncmp(message.c_str(), HELLO_MSG, strlen(HELLO_MSG)) == 0)
        return HELLO_MSG;

    if (strlen(message.c_str()) >= strlen(DISCOVERY_MSG) &&
        strncmp(message.c_str(), DISCOVERY_MSG, strlen(DISCOVERY_MSG)) == 0)
        return DISCOVERY_MSG;

    return "";
}

std::string DiscoveryMessage::get_username() {

    char username[USERNAME_LENGTH] = "";

    if (get_packet_type() == HELLO_MSG) {
        // HELLO_MSG is the smallest string within a discovery message packet
        memcpy(static_cast<void *>(username), static_cast<void *>(&(m_buffer_.at(strlen(HELLO_MSG)))),
               m_buffer_.size() - strlen(HELLO_MSG));
        username[m_buffer_.size() - strlen(HELLO_MSG)] = '\0';
    } else
        throw MessageException("packet is not an Hello Message!\n");

    const auto temp = reinterpret_cast<char *>(username);
    return std::string(temp, strlen(username));
}

std::string DiscoveryMessage::get_message_body() {
    const auto temp = reinterpret_cast<char *>((m_buffer_.data()));
    return std::string(temp, m_buffer_.size());
}


/*********************/
/* 	PROTOCOL MESSAGE */
/*********************/

ProtocolMessage::ProtocolMessage(const __int64 file_size, const FILETIME file_timestamp, const std::string file_name):
	error_code_() {
	// first append the type of the message
	this->message_code_ = protocol::send;

	// insert the request information
	this->request_body_.file_size_ = file_size;
	this->request_body_.file_timestamp_ = file_timestamp;
	this->request_body_.file_name_ = file_name;
}

ProtocolMessage::ProtocolMessage(const protocol::message_code message_code): error_code_() {
	this->message_code_ = message_code;
}

ProtocolMessage::ProtocolMessage(const protocol::error_code error) {
    this->message_code_ = protocol::err;
    this->error_code_ = error;
}

bool ProtocolMessage::compute_request() {

    // TODO check if the buffer contain the exact size of data
    if (m_buffer_.size() < MIN_SIZE_REQUEST_ ||
        m_buffer_.size() > MAX_SIZE_REQUEST_) {
        return false;
    }

    const std::vector<int8_t>::const_iterator begin = m_buffer_.begin() + 4;

    std::vector<int8_t>::const_iterator first = begin;
    std::vector<int8_t>::const_iterator last = first + sizeof(__int64);
    std::vector<int8_t> file_size(first, last);

    memcpy(&request_body_.file_size_, file_size.data(), file_size.size());
    request_body_.file_size_ = ntohll(request_body_.file_size_);
    first = begin + sizeof(__int64);
    last = begin + 2 * sizeof(__int64);
    std::vector<int8_t> file_time_stamp(first, last);

    memcpy(&time_stamp_.QuadPart, file_time_stamp.data(), file_time_stamp.size());
    time_stamp_.QuadPart = ntohll(time_stamp_.QuadPart);
    this->request_body_.file_timestamp_.dwLowDateTime = time_stamp_.LowPart;
    this->request_body_.file_timestamp_.dwHighDateTime = time_stamp_.HighPart;

    first = begin + 2 * sizeof(__int64);
    last = m_buffer_.end();
    std::vector<int8_t> file_name(first, last);

    char name[256];
    memcpy(name, file_name.data(), file_name.size());
    name[file_name.size() - 2] = '\0'; // Needs to remove \r\n
    this->request_body_.file_name_ = std::string(name);

    return true;
}

void ProtocolMessage::compute_packet_type() {

    // use this buffer to load the data packet type conversion
    char packet_type[5];
    packet_type[4] = '\0';

    try {

        if (m_buffer_.empty()) message_code_ = protocol::undefined;

        memcpy(static_cast<void *>(packet_type), static_cast<void *>(&(*m_buffer_.begin())), 4);

        // TODO check if the conversion goes wrong
        message_code_ = protocol::MessageType::get_message_type(std::string(packet_type));
    }
    catch (std::exception &e) {
        message_code_ = protocol::undefined;
    }
}

void ProtocolMessage::prepare_out_packet() {
    switch (message_code_) {
        case protocol::send : {
            prepare_send_message();
        }
            break;
        case protocol::ok : {
            append(protocol::ok);
        }
            break;
        case protocol::err : {
            // append the error and the error code
            append(protocol::err);
            append(error_code_);
        }
            break;
        default:
            break;
    }
}

/*
 * construct a SEND request
 */
void ProtocolMessage::prepare_send_message() {
    // append the send
    append(protocol::send);

    append(static_cast<const char *>(message_body_.c_str()));

    // append the file size
    __int64 fileSize = htonll(this->request_body_.file_size_);
    append(reinterpret_cast<const char *>(&fileSize), sizeof(__int64));

    // append the file timestamp
    time_stamp_.LowPart = this->request_body_.file_timestamp_.dwLowDateTime;
    time_stamp_.HighPart = this->request_body_.file_timestamp_.dwHighDateTime;

    __int64 fileTimestamp = htonll(time_stamp_.QuadPart);
    append(reinterpret_cast<const char *>(&fileTimestamp), sizeof(__int64));

    // append the file name
    append(static_cast<const char *>(this->request_body_.file_name_.c_str()), this->request_body_.file_name_.size());

    // append the request terminator \r\n
    append(static_cast<const char *>(END_MESSAGE_.c_str()));
}


