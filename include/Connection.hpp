#pragma once

#include "stdafx.h"

#include <iostream>
#include <memory>
#include <iomanip>
#include <ctime>
#include <list>

#include "Exceptions.hpp"
#include "Constants.hpp"
#include "SocketBuffer.hpp"

namespace connection {

    class TCPConnection {
    private:
        SOCKET sock_;
        SOCKADDR_IN remote_address_;
        const long sec_ = TIMEOUT_SEC;
        const long usec_ = TIMEOUT_USEC;
        bool alive;

        // definition of the private methods
        size_t readline_unbuffered(char *vptr, int maxlen);

        int read_select(char *read_buffer, int size);

    public:
        TCPConnection(const std::string host, const int port);

        TCPConnection(SOCKET socket, SOCKADDR_IN socket_address);

        void close_connection() const;

        void print_endpoint_info() const;

        bool read_data(std::shared_ptr<SocketBuffer> buffer, int size);

        bool send_data(std::shared_ptr<SendSocketBuffer> buffer);

        bool read_line(std::shared_ptr<SocketBuffer> buffer);
    };

    // definition of the connection pointer type
    typedef std::shared_ptr<TCPConnection> conn_ptr;

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
