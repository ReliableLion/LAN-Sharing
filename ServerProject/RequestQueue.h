#ifndef REQUEST_QUEUE_H
#define REQUEST_QUEUE_H

#include "TCPconnection.h"
#include "RequestInsertionException.h"
#include <string>
#include <memory>
#include <queue>

/*
	definition of a new type called download request
*/
typedef struct {
	std::string fileName;
	std::size_t fileSize;
	std::shared_ptr<TCPconnection_server> connection;
} dwld_request;

#define MAX_QUEUE_ELEMENTS 1000

template <typename T>
class ConcurrentQueue {

private:
	std::queue<T> c_queue;
	int n_values;

public:
	ConcurrentQueue() { n_values = 0; }
	~ConcurrentQueue() {}
	bool insertElement(T element) {
		if (n_values == MAX_QUEUE_ELEMENTS) { return false; }
		c_queue.push(element);
		n_values++;
		return true;
	}

	void popElement(T& element) {
		if (c_queue.empty()) {
			// throw a new exception
		}
		element = c_queue.front();
		c_queue.pop();
		n_values--;
	}

	bool isEmpty() { return c_queue.empty(); }
};


/*
	this class include the information about a request:
	- filename (name of the file + path on the filesystem)
	- size of the file 
	- pointer to the TCP connection
*/
class RequestQueue: public ConcurrentQueue<dwld_request> {
public:
	RequestQueue();
	void insertRequest(std::size_t filesize, std::string fileName, std::shared_ptr<TCPconnection_server> new_connection);
};


#endif // !REQUEST_QUEUE_H
