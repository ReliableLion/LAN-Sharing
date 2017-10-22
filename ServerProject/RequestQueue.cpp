#include "RequestQueue.h"

RequestQueue::RequestQueue() {}

// this method should be protected from concurrent access
void RequestQueue::insertRequest(size_t filesize, std::string fileName, std::shared_ptr<TCPconnection_server> new_connection) {
	 
	dwld_request new_request;

	new_request.connection = new_connection;
	new_request.fileName = fileName;
	new_request.fileSize = filesize;

	incomingRequest.push(new_request);
}

// return the reference of the request 
void RequestQueue::removeRequest(dwld_request& value) {

	if (!incomingRequest.empty()) {
		throw std::exception();
	}
	
	value= incomingRequest.front();
	incomingRequest.pop();
}

bool RequestQueue::isEmpty() {
	return incomingRequest.empty();
}