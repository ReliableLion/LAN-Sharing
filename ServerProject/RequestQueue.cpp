#include "RequestQueue.h"

RequestQueue::RequestQueue() {
	entries_n = 0;
}

// this method should be protected from concurrent access
// return an exception if the number of elements stored in the queue is larger than the maximum 
void RequestQueue::insertRequest(size_t filesize, std::string fileName, std::shared_ptr<TCPconnection_server> new_connection) {
	 
	dwld_request new_request;

	if (entries_n == MAX_QUEUE_ELEMENTS) {
		throw std::exception();
	}

	new_request.connection = new_connection;
	new_request.fileName = fileName;
	new_request.fileSize = filesize;

	incomingRequest.push(new_request);
	entries_n++;
}

// return the reference of the request 
void RequestQueue::removeRequest(dwld_request& value) {

	if (!incomingRequest.empty()) {
		throw std::exception();
	}
	
	value= incomingRequest.front();
	incomingRequest.pop();
	entries_n--;
}

bool RequestQueue::isEmpty() {
	return incomingRequest.empty();
}