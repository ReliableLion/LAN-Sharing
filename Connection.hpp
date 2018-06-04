#pragma once

#include "stdafx.h"

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

        // definition of the private methods
        size_t readline_unbuffered(char *vptr, int maxlen) const;

        int read_select(char *read_buffer, int size) const;

    public:
        TcpConnection(const std::string host, const int port);

        TcpConnection(SOCKET socket, SOCKADDR_IN socket_address);

        void close_connection() const;

        void print_endpoint_info() const;

        bool read_data(std::shared_ptr<SocketBuffer> buffer, int size);

        bool send_data(std::shared_ptr<SendSocketBuffer> buffer) const;

        bool read_line(std::shared_ptr<SocketBuffer> buffer) const;
    };

    // definition of the connection pointer type
    typedef std::shared_ptr<TcpConnection> conn_ptr;

    class Session {
    private:
        std::list<conn_ptr> connection_pool_;

        // the constructor of copy and the assign operator are deleted
        Session(const Session &source) {}

        //Session& operator=(const Session &source) {}

    public:
        Session();

        ~Session();

        void add_connection_to_session();
    };


}
