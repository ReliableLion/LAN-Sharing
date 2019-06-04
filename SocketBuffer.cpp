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

	if (bytes_wrote > CHUNK)
		throw SocketBufferException(std::string("Buffer overflow! Too many bytes wrote!"));

	read_ptr_ = buffer_;
	buffer_size_ = bytes_wrote;
	bytes_already_read = 0;
}

void SocketBuffer::bytes_read(int bytes_read) {

	if (bytes_read < 0)
		return;

	if (bytes_read > buffer_size_ - bytes_already_read)
		throw SocketBufferException(std::string("Buffer overflow! Too many bytes read!"));

	read_ptr_ += bytes_read;
	bytes_already_read += bytes_read;
}

void SocketBuffer::clear() {
    memset(buffer_, 0, MAX_BUFF_);
	read_ptr_ = buffer_;
    buffer_size_ = 0;
	bytes_already_read = 0;
}

char* SocketBuffer::read_from_buffer() {
	return read_ptr_;
}

char* SocketBuffer::write_to_buffer() {
	return buffer_;
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
