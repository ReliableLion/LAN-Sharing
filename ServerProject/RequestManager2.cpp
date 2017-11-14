#include "RequestManager.hpp"
#include "stdafx.h"

//		PUBLIC METHODS

// remember to pass the pointer to the downloadmanager
RequestManager::RequestManager() {
	is_terminated.store(false);
	for (int i = 0; i < maxThreads; i++) {
		threadPool.push_back(std::thread(&RequestManager::processRequest, this));
	}
}

RequestManager::~RequestManager() {
	is_terminated.store(true);
	session::conn_ptr connection;

	while (!connectionQueue.isEmpty()) {
		connectionQueue.popElement(connection);
		connection->closeConnection();
	}
	for (int i = 0; i < maxThreads; i++) {
		threadPool[i].join();
	}
}

bool RequestManager::addConnection(session::conn_ptr newConnection) {
	if (!is_terminated.load()) {
		std::lock_guard<std::mutex> l(mtx);
		if (!connectionQueue.insertElement(newConnection)) {
			return false;			// the queue has reached the max number of element
		}
	}
	else {
		return false;				// this object has been closed
	}
	return true;
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
		session::conn_ptr newConn;
		connectionQueue.popElement(newConn);
		l.unlock();
		processRequest();
	}
}

void RequestManager::processRequest() {
	
	if (!receivePacket()) {			// if the packet is not correctly decode send an message of error

		sendPacket();
		if (!receivePacket()) {
				// print an error and close the connection
		}

	}


}