#pragma once

#include "stdafx.h"

#include <memory>
#include <vector>
#include <cstring>

#include "Exceptions.hpp"

class SocketBuffer {
protected:
    char *buffer_;
    const int max_buff_ = 0;
    int current_size_;
public:
    SocketBuffer();

    void add(const char *data, int size);

    void clear();

    void replace(const char *data, int size);

    virtual char *get_buffer();

    int get_size();

    int get_max_size();
};


class SendSocketBuffer : public SocketBuffer {
private:
    int send_position_;
    char *read_buffer_;
public:
    SendSocketBuffer() : SocketBuffer(), send_position_(0) { read_buffer_ = buffer_; };

    void send(int n);

    char *get_buffer() override;

    int get_bytes_sent();

    int get_remaining_bytes();
};
