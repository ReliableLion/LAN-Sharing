#ifndef REQUEST_QUEUE_H
#define REQUEST_QUEUE_H

#include "TCPconnection.h"
#include <string>
#include <memory>
#include <queue>

typedef struct {
	std::string fileName;
	size_t fileSize;
	std::shared_ptr<TCPconnection> connection;
} request;

/*
	this class include the information about a request:
	- filename (name of the file + path on the filesystem)
	- size of the file 
	- pointer to the TCP connection
*/
class RequestQueue {

private:
	std::queue<request> incomingRequest;

public:
	RequestQueue();
	void insertRequest(size_t filesize, std::string fileName, std::shared_ptr<TCPconnection> new_connection);
	void removeRequest(request& value);
	bool isEmpty();
};


#endif // !REQUEST_QUEUE_H
