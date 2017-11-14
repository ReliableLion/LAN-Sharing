#ifndef REQUEST_INSERTION_EXCEPTION_H
#define REQUEST_INSERTION_EXCPETION_H

#include <exception>

class RequestInsertionException : public std::exception {
public:
	virtual const char* what() const throw() {
		return "is not possible to insert the request into the queue";
	}
};

#endif // !REQUEST_INSERTION_EXCEPTION_H

