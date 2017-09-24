#include "RequestQueue.h"

RequestQueue::RequestQueue() {}

// this method should be protected from concurrent access
void RequestQueue::insertRequest(size_t filesize, std::string fileName, std::shared_ptr<TCPconnection> new_connection) {
	 
	request new_request;

	new_request.connection = new_connection;
	new_request.fileName = fileName;
	new_request.fileSize = filesize;

	incomingRequest.push(new_request);
}

// return the reference of the request 
request& RequestQueue::removeRequest() {

	if (!incomingRequest.empty()) {
		throw std::exception();
	}
	
	request req = incomingRequest.front();
	incomingRequest.pop();
	return req;
}

bool RequestQueue::isEmpty() {
	return incomingRequest.empty();
}