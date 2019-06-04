#include <iostream>

#include "SocketBuffer.hpp"
#include "Exceptions.hpp"

SocketBuffer::SocketBuffer() : buffer_size_(0), bytes_already_read(0) { 
	buffer_ = new char[MAX_BUFF_];
	read_ptr_ = buffer_;
}

SocketBuffer::~SocketBuffer() {
	delete[] buffer_;
}

void SocketBuffer::add(const char *data, const int size) {

    if (size < 1) 
		return;

    if (buffer_size_ + size > MAX_BUFF_) 
		throw SocketBufferException(std::string("the buffer doesn't have enought space to add data"));
   
    // copy the content of the buffer passed as parameter to the internal buffer
    memcpy(buffer_ + buffer_size_, data, size);
    buffer_size_ += size;
}

void SocketBuffer::replace(const char *data, const int size) {

	if (size < 1)
		return;

	if (size > MAX_BUFF_)
		throw SocketBufferException(std::string("the size of the data to be insert is greater than the buffer size"));

	// questo metodo non sarebbe necessario chiamarlo visto che la memcpy sovrascrive il buffer
	// this->clear();
	memcpy(buffer_, data, size);
	read_ptr_ = buffer_;
    buffer_size_ = size;
	bytes_already_read = 0;
}

void SocketBuffer::bytes_wrote(int bytes_wrote) {

	if (bytes_wrote < 0)
		return;

	if (bytes_wrote > buffer_size_)
		throw SocketBufferException(std::string("buffer overflow"));

	read_ptr_ = buffer_;
	buffer_size_ = bytes_wrote;
	bytes_already_read = 0;
}

void SocketBuffer::bytes_read(int bytes_read) {

	if (bytes_read < 0)
		return;

	if (bytes_read > buffer_size_ - bytes_already_read)
		throw SocketBufferException(std::string("buffer overflow"));

	read_ptr_ += bytes_read;
	bytes_already_read += bytes_read;
}

void SocketBuffer::clear() {
    memset(buffer_, 0, MAX_BUFF_);
	read_ptr_ = buffer_;
    buffer_size_ = 0;
	bytes_already_read = 0;
}

char *SocketBuffer::read() {
	if (buffer_size_ != bytes_already_read) 
		return read_ptr_;
	
	// throw a new buffer exception
	throw SocketBufferException(std::string("impossible to read more data than the buffer can"));
}

char* SocketBuffer::read_to_buffer() {
	return read_ptr_;
}

char* SocketBuffer::write_to_buffer() {
	return buffer_;
}

void SocketBuffer::update_read_ptr(int size) {
	if (size < 0)
		return;

	if (size > buffer_size_ - bytes_already_read)
		throw SocketBufferException(std::string("buffer overflow"));

	read_ptr_ += size;
	bytes_already_read += size;
}

void SocketBuffer::read_all_bytes() {
	bytes_already_read = buffer_size_;
	read_ptr_ += buffer_size_;
}

int SocketBuffer::get_max_size() const {
    return MAX_BUFF_;
}

int SocketBuffer::get_size() const {
    return buffer_size_;
}

int SocketBuffer::get_remaining_bytes() {
	return buffer_size_ - bytes_already_read;
}

/*
	SendSoocketBuffer implementation
*/

/*void SendSocketBuffer::send(int size) {

	if (size < 1)
        return;

	if (size > buffer_size_ || size > buffer_size_ - bytes_sent_) 
		throw SocketBufferException(std::string("impossible to read more bytes than the buffer capacity"));
   
    send_ptr_ += size;
    buffer_size_ += size;
}

int SendSocketBuffer::get_bytes_sent() const {
    return bytes_sent_;
}

char *SendSocketBuffer::get_remaining_data() {
    return send_ptr_;
}

int SendSocketBuffer::get_remaining_bytes() const {
    return buffer_size_ - bytes_sent_;
}*/

/*
	RecvSocketBuffer implementation 
*/

/*
void RecvSocketBuffer::receive(const char *data, int size) {
	add(data, size);
}

int RecvSocketBuffer::get_bytes_received() {
	return bytes_received_;
}*/

/*
	SRSocketBuffer implementation
*/
/*
SRSocketBuffer::SRSocketBuffer() : bytes_received_(0), bytes_sent_(0) {
	recv_ptr_ = buffer_;
	send_ptr_ = buffer_;
}

void SRSocketBuffer::receive(int size) {

	if (size < 1)
		return;

}

void SRSocketBuffer::send(char *data, int size) {

}

int SRSocketBuffer::get_bytes_received() {
	return bytes_received_;
}

int SRSocketBuffer::get_bytes_sent() {
	return bytes_sent_;
} */