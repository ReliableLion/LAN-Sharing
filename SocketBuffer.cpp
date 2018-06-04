#include "stdafx.h"

#include "SocketBuffer.hpp"
#include "Exceptions.hpp"

SocketBuffer::SocketBuffer() : current_size_(0) {
    buffer_ = new char(MAX_BUFF_);
}

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

char *SocketBuffer::get_buffer() {
    return buffer_;
}

void SendSocketBuffer::send(int n) {
    if (n < 1)
        return;

    if (n > current_size_ || n > send_position_ - current_size_) {
        //TODO lauch a new exception
    } else {
        read_buffer_ += n;
        send_position_ += n;
    }
}

int SendSocketBuffer::get_bytes_sent() const {
    return send_position_;
}

char *SendSocketBuffer::get_buffer() {
    return read_buffer_;
}


int SendSocketBuffer::get_remaining_bytes() const {
    return current_size_ - send_position_;
}










