#pragma once
#include <exception>

class TimeoutException : public std::exception {
	virtual const char* what() const throw() {
		return "the connection reached the timeout";
	}
};