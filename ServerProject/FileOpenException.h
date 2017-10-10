#ifndef FILE_OPEN_EXCEPTION_H
#define FILE_OPEN_EXCEPTION_H

#include <exception>

/*
	this class is used to laucnh new exception during the file opening
*/
class FileOpenException : public std::exception {
	virtual const char* what() const throw() {
		return "is not possible to open/close the file";
	}
};

#endif // !FILE_OPEN_EXCEPTION_H
