#pragma once

#include <exception>
#include <sstream>

namespace udp_exception {
    class udp_exception : public std::runtime_error {
    public:
        explicit udp_exception(const std::string str) : runtime_error("UDP Service Exception"), error_message_(str) {};

        const char *what() const throw() override { return error_message_.c_str(); }

    private:
        std::string error_message_;
    };
}

class tcp_exception : public std::exception {
public:
    explicit tcp_exception(const std::string str) : error_message_(str) {};

    const char *what() const throw() override { return error_message_.c_str(); }

private:
    std::string error_message_;
};

class message_exception : public std::exception {
public:
    explicit message_exception(const std::string str) : error_message_(str) {};

    const char *what() const throw() override { return error_message_.c_str(); }

private:
    std::string error_message_;
};


class FileTransmissionException : public std::exception {
public:
    explicit FileTransmissionException(const int error) {
        this->error_ = error;
    }

    const char *what() const throw() override {
        std::stringstream s;
        s << "problem with socket, error: " << error_;
        return s.str().c_str();
    }

private:
    int error_;
};

class SocketException : public std::exception {
public:
    explicit SocketException(const int error) {
        this->error_ = error;
    }

    const char *what() const throw() override {
        std::stringstream s;
        s << "problem with socket, error: " << error_;
        return s.str().c_str();
    }

private:
    int error_;
};

class FileOpenException : public std::exception {
public:
    const char *what() const throw() override {
        return "is not possible to open/close the file";
    }
};

class FileWriteException : public std::exception {
    const char *what() const throw() override {
        return "is not possible to write the file";
    }
};

class TimeoutException : public std::exception {
    virtual const char *what() const throw() {
        return "the connection reached the timeout";
    }
};

class SocketBufferException : public std::exception {
    virtual const char *what() const throw() {
        return "error for the buffer";
    }
};

class ConnectionCloseException : public std::exception {
    virtual const char *waht() const throw() {
        return "connection has been closed";
    }
};