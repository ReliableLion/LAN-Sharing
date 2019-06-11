#pragma once

#include "pch.h"
#include <memory>

class FileHandler;

namespace connection {

    class TcpConnection {
    private:
        SOCKET sock_;
        SOCKADDR_IN remote_address_;
        const long SEC_ = TIMEOUT_SEC;
        const long USEC_ = TIMEOUT_USEC;
        bool alive_;

		char *receive_buffer_;
		char *reading_buffer;

        size_t readline_unbuffered(char *vptr, int maxlen);

    public:
        TcpConnection(const std::string host, const int port);

        TcpConnection(SOCKET connected_socket, SOCKADDR_IN remote_address);

		~TcpConnection();

        void close_connection() const;

        bool read_data(std::shared_ptr<SocketBuffer> buffer);

		void select_connection();

		void select_write_connection();

        bool send_data(std::shared_ptr<SocketBuffer> buffer);

		bool read_n_data(std::shared_ptr<SocketBuffer> buffer, int n);

		int read_file(size_t file_size, FileHandler &temporary_file);

		int send_file(HANDLE file_handle, DWORD file_size);

		// bool read_data2(int size);

		// char* getReceiveBuffer();

        bool read_line(std::shared_ptr<SocketBuffer> buffer);

		bool get_connection_status();

		void print_endpoint_info() const;

		SOCKET get_handle_socket();
    };

    // definition of the connection pointer type
    typedef std::shared_ptr<TcpConnection> connection_ptr;
}
