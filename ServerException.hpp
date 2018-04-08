#pragma once
#include <exception>

enum ServerError {
	WRONG_PORT,
	BINDING_ERROR
};

class ServerException: std::exception {
public:
	ServerException(ServerError);
	virtual const char* what() const throw() {}

};
