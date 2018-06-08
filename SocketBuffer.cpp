#include "stdafx.h"

#include "SocketBuffer.hpp"
#include "Exceptions.hpp"
#include <iostream>

void SocketBuffer::add(const char *data, const int size) {
    if (size == 0)
        return;

    if (current_size_ + size > MAX_BUFF_) {
        throw SocketBufferException();
    }

    // copy the content of the buffer passed as parameter to the internal buffer
    memcpy(buffer_ + current_size_, data, size);
    current_size_ += size;
}

void SocketBuffer::replace(const char *data, const int size) {
    if (size < 1)
        return;

    if (size > MAX_BUFF_ || size < 0) {
        // TODO lanciare eccezione
		throw SocketBufferException();
    }

    memset(buffer_, 0, MAX_BUFF_);
    memcpy(buffer_, data, size);
    current_size_ = size;
}


void SocketBuffer::clear() {
    memset(buffer_, 0, MAX_BUFF_);
    current_size_ = 0;
}

int SocketBuffer::get_max_size() const {
    return MAX_BUFF_;
}

int SocketBuffer::get_size() const {
    return current_size_;
}

char *SocketBuffer::get_remaining_data() {
    return buffer_;
}

void SendSocketBuffer::send(int n) {
    if (n < 1)
        return;

    if (n > current_size_ || n > current_size_ - send_position_) {
        //TODO launch a new exception
	    std::cout << "ERROR in socketbuffer send" << std::endl;
    } else {
        remaining_data_buffer_ += n;
        send_position_ += n;
    }
}

int SendSocketBuffer::get_bytes_sent() const {
    return send_position_;
}

char *SendSocketBuffer::get_remaining_data() {
    return remaining_data_buffer_;
}


int SendSocketBuffer::get_remaining_bytes() const {
    return current_size_ - send_position_;
}










