#pragma once
#include <exception>
#include <iostream>
#include <sstream>

namespace udp_exception {
	class udpException: public std::exception
	{
	public:
		udpException(std::string str) : errorMessage(str) {};
		const char* what() const throw() { return errorMessage.c_str(); }

	private:
		std::string errorMessage;
	};
}

class messageException : public std::exception
{
public:
	messageException(std::string str) : errorMessage(str) {};
	const char* what() const throw() { return errorMessage.c_str(); }

private:
	std::string errorMessage;
};

class SocketException : public std::exception {
private:
	int error;
public:
	SocketException(int error) {
		this->error = error;
	}

	virtual const char* what() const throw() {
		std::stringstream s;
		s << "problem with socket, error: " << error;
		return s.str().c_str();
	}
};