#include "RequestManager.hpp"

//		PUBLIC METHODS

RequestManager::RequestManager() : dw_man() {
	is_terminated.store(false);
	for (int i = 0; i < maxThreads; i++) {
		threadPool.push_back(std::thread(&RequestManager::processRequest, this));
	}
}

RequestManager::~RequestManager() {
	is_terminated.store(true);			// this flag is used to stop the threads computation
	session::conn_ptr connection;		

	while (!connectionQueue.isEmpty()) {			// iterate the entire queue and close the pending connections
		connectionQueue.popElement(connection);
		connection->closeConnection();
	}

	for (int i = 0; i < maxThreads; i++) {			// close all the threads
		cv.notify_all();
		threadPool[i].join();						// "unlock" the threads that wait on the condition variable
	}
}

bool RequestManager::addConnection(session::conn_ptr newConnection) {
	if (!is_terminated.load()) {
		std::lock_guard<std::mutex> l(mtx);
		if (!connectionQueue.insertElement(newConnection)) {
			return false;			// the queue has reached the max number of element
		}
		else {
			return true;
		}
	}
	else {
		return false;				// this object has been closed
	}
}

//		PRIVATE METHODS

void RequestManager::extractConnection() {
	session::conn_ptr newConnection;
	std::unique_lock<std::mutex> ul(mtx, std::defer_lock);

	while (1) {
		ul.lock();

		cv.wait(ul, [this]() {
			return (!connectionQueue.isEmpty() || is_terminated.load());
		});
		
		if (is_terminated.load()) break;
		connectionQueue.popElement(newConnection);
		ul.unlock();

		processRequest();
	}
}

void RequestManager::processRequest() {
	
	/*if (!receivePacket()) {			// if the packet is not correctly decode send an message of error

		sendPacket();
		if (!receivePacket()) {
				// print an error and close the connection
		}

	}*/

	bool received_correctly = false;
	bool connection_closed = false;
	int i = 0;

	// TODO declare here the requestManager and ReplyManager
	try{
		do {

			/*
			-if the packet is received correctly, write the reply and set received correclty true
				-if the request is accepted pass the connection to the downloadmanager
				-if the requets is not accepted send an error
			-if the packet is not received correctly, send an error and set the index + 1
			-if the connection is closed, write a message
			*/


		} while (received_correctly || connection_closed || i == maxRequestAttempts);

		if (!received_correctly) {

		} else 
		if (i == maxRequestAttempts) {
			// TODO close the connection
		} else 
		if (connection_closed){
			std::cout << "connection closed by peer" << std::endl;
		}
		
	}
	catch (TimeoutException &e) {
		// TODO close the connection 
	}
	catch (SocketException &e) {
		// TODO close the connection
	}
}

bool sendReply(session::conn_ptr conn) {
	return true;
}