#include "RequestQueue.h"


RequestQueue::RequestQueue() : ConcurrentQueue() {}

// this method should be protected from concurrent access
// return an exception if the number of elements stored in the queue is larger than the maximum 
bool RequestQueue::insertRequest(size_t filesize, std::string fileName, std::shared_ptr<TCPconnection_server> new_connection) {
	 
	dwld_request new_request;

	new_request.connection = new_connection;
	new_request.fileName = fileName;
	new_request.fileSize = filesize;

	return insertElement(new_request);
}