#ifndef TCP_READ_EXCEPTION_H
#define TCP_READ_EXCEPTION_H

#include <exception>

class TCPreadException : public std::exception {
public: 
	virtual const char* what() const throw() {
		return "is impossible to read the data from the connection TCP";
	}
};

#endif
