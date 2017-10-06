#ifndef FILE_WRITE_EXCEPTION_H
#define FILE_WIRTE_EXCEPTION_H 

#include <exception>

class FileWriteException : public std::exception {
	virtual const char* what() const throw() {
		return "is not possible to write the file";
	}
};

#endif // !FILE_WRITE_EXCEPTION_H
