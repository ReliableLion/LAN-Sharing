#pragma once
#include <string>
#include <exception>
#include <sstream>

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