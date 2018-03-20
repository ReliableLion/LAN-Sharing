#pragma once
#include <exception>
#include <sstream>

namespace udp_exception {
	class udp_exception : public std::exception
	{
	public:
		explicit udp_exception(const std::string str) : error_message_(str) {};
		const char* what() const throw() override { return error_message_.c_str(); }

	private:
		std::string error_message_;
	};
}

class message_exception : public std::exception
{
public:
	explicit message_exception(const std::string str) : error_message_(str) {};
	const char* what() const throw() override { return error_message_.c_str(); }

private:
	std::string error_message_;
};

class transmit_file_exception : public std::exception
{
public:
	explicit transmit_file_exception(const int error) {
		this->error_ = error;
	}
	const char* what() const throw() override {
		std::stringstream s;
		s << "problem with socket, error: " << error_;
		return s.str().c_str();
	}

private:
	int error_;
};

class socket_exception : public std::exception {

public:
	explicit socket_exception(const int error) {
		this->error_ = error;
	}

	const char* what() const throw() override {
		std::stringstream s;
		s << "problem with socket, error: " << error_;
		return s.str().c_str();
	}

private:
	int error_;
};

class file_open_exception : public std::exception {
public:
	const char* what() const throw() override {
		return "is not possible to open/close the file";
	}
};

class file_write_exception : public std::exception {
	const char* what() const throw() override {
		return "is not possible to write the file";
	}
};