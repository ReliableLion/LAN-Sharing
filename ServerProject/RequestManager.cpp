#include "RequestMananger.h"

RequestManager::RequestManager() {

	for (int i = 0; i < MAX_THREADS; i++) {
		t[i] = std::thread();
	}

	terminate.store(false);
}

RequestManager::~RequestManager() {
	shutdown();
}

void RequestManager::shutdown() {
	std::shared_ptr<TCPconnection_server> conn;

	terminate.store(true);

	for (int i = 0; i < MAX_THREADS; i++) {
		t[i].join();
	}

	// if shutdown method is called then all the connections that are in connection_pool are closed
	while (connection_pool.empty()) {
		conn = connection_pool.front();
		conn->closeConnection();
		connection_pool.pop();
	}

}

void RequestManager::addRequest(std::shared_ptr<TCPconnection_server> conn) {

	// check if the server has turned off
	if (terminate.load()) {
		throw std::exception();
	}

	// add the request into the queue
	std::lock_guard <std::mutex> l(mtx1);
	connection_pool.push(conn);
	cv.notify_all();
}

void RequestManager::processRequest() {

	std::shared_ptr<TCPconnection_server> conn;

	while (1) {

		bool t = terminate.load();
		if (!t) { break; }

		std::unique_lock<std::mutex> l(mtx1);
		cv.wait(l, [this, t] () {
			return (!connection_pool.empty() || t);
		});

		if (t) { break; }

		conn = connection_pool.front();
		connection_pool.pop();

		l.unlock();

		// here receive the client request and decode it in order to begin the file download

	}

}

// wiht handshake i mean the time window between the request and the reply
void RequestManager::requestHandShake(std::shared_ptr<TCPconnection_server> conn) {

	RequestMessage r_msg;
	ReplyMsg reply_msg;

	// TODO	implement a timeout mechanism in order to close connections that are open but unused

	// check if the connection is alive
	if (!conn->checkConnection()) {

	}

	try {
		// use the reveive method of connection to read the client incoming request
		conn->readRequest(r_msg);

		//TODO	devo aspettare davide che finisca la class messagecontainer
		

		conn->writeReply(reply_msg);
	}
	catch (std::exception &e) {
		e.what();
	}

		 
}


