#pragma once

#include <cstring>

#include "Constants.hpp"

class SocketBuffer {
protected:
    char *buffer_;		
	char *read_ptr_;
	unsigned int buffer_size_;
	unsigned int bytes_already_read;
	const unsigned int MAX_BUFF_ = CHUNK;
public:
	SocketBuffer();

	~SocketBuffer();

    void add(const char *data, int size);

	void replace(const char *data, int size);
	
	void bytes_read(int bytes_read);

	void bytes_wrote(int bytes_wrote);

	char *read_from_buffer();
	
	char* write_to_buffer();

	void read_all_bytes();

    void clear();

	int get_remaining_bytes();

    int get_size() const;

    int get_max_size() const;
};

/*
class SendSocketBuffer : public SocketBuffer {
private:
    unsigned int bytes_sent_;		// number of bytes already sent 
    char *send_ptr_;				// variable that point to the first byte to be sent
public:
    SendSocketBuffer() : SocketBuffer(), bytes_sent_(0) { send_ptr_ = buffer_; };

    void send(int size);

    char *get_remaining_data() override;

    int get_bytes_sent() const;

    int get_remaining_bytes() const;
};

class RecvSocketBuffer : public SocketBuffer {
private: 
	unsigned int bytes_received_;	// number of bytes received
	char *recv_ptr_;				// variable that points to the first availale location to be wrote
public:	
	RecvSocketBuffer() : SocketBuffer(), bytes_received_(0) { recv_ptr_ = buffer_; };

	void receive(const char *data, int size);

	int get_bytes_received();
};

class SRSocketBuffer : public SocketBuffer {
private:
	unsigned int bytes_received_;
	unsigned int bytes_sent_;
	char *recv_ptr_;
	char *send_ptr_;

public:
	SRSocketBuffer();

	void receive(int size);
	void send(char *data, int size);
	int get_bytes_received();
	int get_bytes_sent();
};*/
