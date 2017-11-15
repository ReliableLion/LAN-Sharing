#include "RequestManager.hpp"
#include "stdafx.h"

//		PUBLIC METHODS

RequestManager::RequestManager() {
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

	for (int i = 0; i < maxThreads; i++) {			// wait the threads termination
		threadPool[i].join();
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
	std::unique_lock<std::mutex> l(mtx, std::defer_lock);

	while (1) {
		l.lock();

		cv.wait(l, [this]() {
			return (!connectionQueue.isEmpty() || is_terminated.load());
		});
		
		if (is_terminated.load()) break;
		connectionQueue.popElement(newConnection);
		l.unlock();

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
	int i = 0;

	// TODO declare here the requestManager and ReplyManager
	try{
		do {

			/*
			-if the packet is received correctly, write the reply and set received correclty true
				-if the request is accepted pass the connection to the downloadmanager
				-if the requets is not accepted send an error
			-if the packet is not received correctly, send an error and set the index + 1
			*/


		} while (received_correctly || i == maxRequestAttempts);

		if (!received_correctly) {

		}
		else {
			// TODO close the connection
		}

	}
	catch (TimeoutException &e) {
		// TODO close the connection 
	}
	catch (SocketException &e) {
		// TODO close the connection
	}
}