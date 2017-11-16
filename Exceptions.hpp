#pragma once
#include <exception>
#include <iostream>

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