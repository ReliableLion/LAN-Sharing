#include "SocketBuffer.hpp"
#include "Exceptions.hpp"

SocketBuffer::SocketBuffer() : current_size_(0) 
{
	buffer_ = new char(max_buff_);
}

void SocketBuffer::add(const char *data, int size) 
{
	if (size == 0)
		return;

	if (current_size_ + size > max_buff_)
	{
		throw SocketBufferException();
	}
	
	// copy the content of the buffer passed as parameter to the internal buffer
	memcpy(buffer_ + current_size_, data, size);
	current_size_ += size;
}

void SocketBuffer::replace(const char* data, int size)
{
	if (size < 1)
		return;

	if (size > max_buff_ || size < 0 )
	{
		// TODO lanciare eccezione
	}

	memset(buffer_, 0, max_buff_);
	memcpy(buffer_, data, size);
	current_size_ = size;
}


void SocketBuffer::clear()
{
	memset(buffer_, 0, max_buff_);
	current_size_ = 0;
}

int SocketBuffer::get_max_size()
{
	return max_buff_;
}

int SocketBuffer::get_size()
{
	return current_size_;
}

char* SocketBuffer::get_buffer()
{
	return buffer_;
}

void SendSocketBuffer::send(int n)
{
	if (n < 1)
		return;

	if (n > current_size_ || n > send_position_ - current_size_)
	{
		//TODO lauch a new exception
	}
	else
	{
		read_buffer_ += n;
		send_position_ += n;
	}
} 

int SendSocketBuffer::get_bytes_sent()
{
	return send_position_;
}

char* SendSocketBuffer::get_buffer()
{
	return read_buffer_;
}


int SendSocketBuffer::get_remaining_bytes()
{
	return current_size_ - send_position_;
}










