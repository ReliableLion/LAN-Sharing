#ifndef BAD_REQUEST_H 
#define BAD_REQUEST_H

#include <exception>

class BadRequest : std::exception {
public: 
	virtual const char* what() const throw() {
		return "server cannot recognize the client request";
	}
};

#endif // !BAD_REQUEST_H 

