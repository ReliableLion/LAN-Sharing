#include <sstream>
#include <iostream>

#include "Message.hpp"
#include "Exceptions.hpp"
#include "WindowsFileHandler.hpp"

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

	prepare_out_packet();
}

ProtocolMessage::ProtocolMessage(const protocol::message_code message_code): error_code_() {
	this->message_code_ = message_code;
	prepare_out_packet();
}

ProtocolMessage::ProtocolMessage(const protocol::error_code error) {
    this->message_code_ = protocol::err;
    this->error_code_ = error;
	prepare_out_packet();
}

/*
 * This method compute the data contained in a packet which type is SEND
 */
bool ProtocolMessage::compute_send_request() {

    // TODO check if the buffer contain the exact size of data
    if (m_buffer_.size() < MIN_SIZE_REQUEST_ ||
        m_buffer_.size() > MAX_SIZE_REQUEST_) {
        return false;
    }

	// Check if the string is terminated correctly
	if (!stream_.str().find("\r\n"))
		return false;

	// Clean the stream
	stream_.str(std::string());

	// Copy the buffer within the stream without 'SEND '
	stream_ << m_buffer_.data() + 5;

	// Get file size, separated by ' ' from the timestamp
	std::string message;
	std::getline(stream_, message, ' ');
	this->request_body_.file_size_ = ntohll(strtoll(message.c_str(), nullptr, 0));

	// Get the timestamp
	std::getline(stream_, message, ' ');
	time_stamp_.QuadPart = ntohll(strtoll(message.c_str(), nullptr, 0));
	this->request_body_.file_timestamp_.dwLowDateTime = time_stamp_.LowPart;
	this->request_body_.file_timestamp_.dwHighDateTime = time_stamp_.HighPart;

	// Get the filename
	std::getline(stream_, this->request_body_.file_name_, '\r');

	if (this->request_body_.file_name_.size() > 256)
		return false;

    return true;
}

void ProtocolMessage::compute_packet_type() {

    try {

		stream_.str(std::string());
		stream_ << m_buffer_.data();

        if (m_buffer_.empty() && !stream_.str().find("\r\n")) message_code_ = protocol::undefined;

		// Clean the stream
		std::string message = stream_.str().substr(0, 4);

        message_code_ = protocol::MessageType::get_message_type(message);

		if(message_code_ == protocol::err) {
			error_code_ = protocol::MessageType::get_error_type(stream_.str().substr(4, m_buffer_.size()));
		}
    }
    catch (std::exception &e) {
		std::cout << "EXCEPTION MESSAGE: " << e.what() << std::endl;
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
			stream_.str(std::string());
			stream_ << END_MESSAGE_;
			append(stream_.str());
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

	stream_.str(std::string());

    // append the file size
	const __int64 file_size = htonll(this->request_body_.file_size_);
	stream_ << ' ' << file_size << ' ';

    // append the file timestamp
    time_stamp_.LowPart = this->request_body_.file_timestamp_.dwLowDateTime;
    time_stamp_.HighPart = this->request_body_.file_timestamp_.dwHighDateTime;

	const __int64 file_timestamp = htonll(time_stamp_.QuadPart);
	stream_ << file_timestamp << ' ';

    // append the file name
	stream_ << this->request_body_.file_name_;

    // append the request terminator \r\n
	stream_ << END_MESSAGE_;

	append(stream_.str());
}


