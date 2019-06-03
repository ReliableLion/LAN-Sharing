#pragma once

#include <memory>
#include <iomanip>
#include <ctime>
#include <list>

#include "Constants.hpp"
#include "SocketBuffer.hpp"

namespace connection {

    class TcpConnection {
    private:
        SOCKET sock_;
        SOCKADDR_IN remote_address_;
        const long SEC_ = TIMEOUT_SEC;
        const long USEC_ = TIMEOUT_USEC;
        bool alive_;

		char *receive_buffer_;

        size_t readline_unbuffered(char *vptr, int maxlen);

        int read_select(char *read_buffer, int size);

    public:
        TcpConnection(const std::string host, const int port);

        TcpConnection(SOCKET connected_socket, SOCKADDR_IN remote_address);

		~TcpConnection();

        void close_connection() const;

        bool read_data(std::shared_ptr<SocketBuffer> buffer);

        bool send_data(std::shared_ptr<SocketBuffer> buffer);

		bool read_n_data(std::shared_ptr<SocketBuffer> buffer, int n);

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
