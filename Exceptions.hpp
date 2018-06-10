#pragma once

#include <exception>
#include <sstream>
#include <iostream>

namespace udp_exception {

    class UdpException : public std::runtime_error {
    public:
        explicit UdpException(const std::string str) : runtime_error("UDP Service Exception: " + str) {};
    };

	class UdpShutdownException : public std::runtime_error {
	public:
		explicit UdpShutdownException() : runtime_error("UDP Service Shutdown Exception") {};
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


class TcpException : public std::runtime_error {
public:
    explicit TcpException(const std::string str) :runtime_error("TCP Exception: " + str) {};
};

class MessageException : public std::runtime_error {
public:
    explicit MessageException(const std::string str) : runtime_error("Message Exception: " + str) {};
};


class FileTransmissionException : public std::runtime_error {
public:
	explicit FileTransmissionException(const int error) : runtime_error("Problem with file transmission Exception: " + error) {}
};

class SocketException : public std::runtime_error {
public:
    explicit SocketException(const int error): runtime_error(("Problem with socket Exception: " + error)) {}
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

class WindowsFileHandlerException : public std::runtime_error {
public:
	explicit WindowsFileHandlerException(const std::string file_name): runtime_error("Cannot open: " + file_name)  {}
};
