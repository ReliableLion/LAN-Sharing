#pragma once

#include <exception>
#include <sstream>

namespace udp_exception {

    class UdpException : public std::runtime_error {
    public:
        explicit UdpException(const std::string str) : runtime_error("UDP Service Exception"), error_message_(str) {};

        const char *what() const throw() override { return error_message_.c_str(); }

    private:
        std::string error_message_;
    };
}

class ServerException : public std::exception {
public:
	const char *what() const throw() override {
		return msg.c_str();
	}
private: 
	const std::string msg = "impossible to create an instance of the server ";
};


class TcpException : public std::exception {
public:
    explicit TcpException(const std::string str) : error_message_(str) {};

    const char *what() const throw() override { return error_message_.c_str(); }

private:
    std::string error_message_;
};

class MessageException : public std::exception {
public:
    explicit MessageException(const std::string str) : error_message_(str) {};

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

	explicit SocketException(const int error) : error_(error) {};

    const char *what() const throw() override {
        std::stringstream s;
        s << msg_ << error_;
        return s.str().c_str();
    }

private:
    int error_;
	const std::string msg_ = "problem with socket, error: ";
};

class FileOpenException : public std::exception {
public:
    const char *what() const throw() override {
        return "is not possible to open/close the file";
    }
};

class FileWriteException : public std::exception {
public:
    const char *what() const throw() override {
        return "is not possible to write the file";
    }
};

class TimeoutException : public std::exception {
	const char *what() const throw() override {
        return "the connection reached the timeout";
    }
};

class SocketBufferException : public std::exception {
public:
	SocketBufferException(std::string msg) : user_msg_(msg) {};

	const char *what() const throw() override {
		return user_msg_.c_str();
    }

private:
	std::string user_msg_;
};

class ConnectionCloseException : public std::exception {
public:
    virtual const char *waht() const throw() {
        return "connection has been closed";
    }
};