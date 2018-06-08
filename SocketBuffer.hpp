#pragma once

#include <cstring>
#include "Constants.hpp"

class SocketBuffer {
protected:
    char *buffer_;
    const int MAX_BUFF_ = CHUNK;
    int current_size_;
public:
	virtual ~SocketBuffer() = default;
	
	SocketBuffer(): current_size_(0) { buffer_ = new char(MAX_BUFF_);};

    void add(const char *data, int size);

    void clear();

    void replace(const char *data, int size);

    virtual char *get_remaining_data();

    int get_size() const;

    int get_max_size() const;
};


class SendSocketBuffer : public SocketBuffer {
private:
    int send_position_;
    char *remaining_data_buffer_;
public:
    SendSocketBuffer() : SocketBuffer(), send_position_(0) { remaining_data_buffer_ = buffer_; };

    void send(int n);

    char *get_remaining_data() override;

    int get_bytes_sent() const;

    int get_remaining_bytes() const;
};
