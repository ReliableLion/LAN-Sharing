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
