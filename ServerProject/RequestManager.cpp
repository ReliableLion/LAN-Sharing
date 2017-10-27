#include "RequestMananger.h"

// when a new object ReqeuestManager is instantiated the Server pass as reference the DownloadManager
RequestManager::RequestManager(std::shared_ptr<DownloadManager> d_man_ptr) {
	this->d_man_ptr = d_man_ptr;

	for (int i = 0; i < MAX_THREADS; i++) {
		t[i] = std::thread();
	}

	terminate.store(false);

	// the downloader class set up all the thread in order to begin the file download
	this->d_man_ptr->setupDownloader();
}

RequestManager::~RequestManager() {
	shutdown();
}

void RequestManager::shutdown() {
	terminate.store(true);

	for (int i = 0; i < MAX_THREADS; i++) {
		t[i].join();
	}
}

void RequestManager::closeConnections() {
	std::shared_ptr<TCPconnection_server> conn;
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

void RequestManager::_processRequest() {

	std::shared_ptr<TCPconnection_server> conn;

	while (1) {

		bool t = terminate.load();

		std::unique_lock<std::mutex> l(mtx1);
		cv.wait(l, [this, t] () {
			return (!connection_pool.empty() || t);
		});

		if (t) { break; }

		conn = connection_pool.front();
		connection_pool.pop();

		l.unlock();

		// here we receive the client request and decode it in order to begin the file download
		_requestHandShake(conn);

	}

}

// wiht handshake i mean the time window between the request and the reply
void RequestManager::_requestHandShake(std::shared_ptr<TCPconnection_server> conn) {

	RequestMessage r_msg;
	ReplyMsg reply_msg;
	__int64 fileSize = 0;
	std::string filename = "";

	// TODO	implement a timeout mechanism in order to close connections that are open but unused

	// check if the connection is alive
	if (!conn->checkConnection()) {

	}

	try {
		// use the reveive method of connection to read the client incoming request
		conn->readRequest(r_msg);

		//TODO	devo aspettare davide che finisca la class messagecontainer

		// TODO	i need to use the message class methods to decode the content of the request

		if (_checkParameter(fileSize, filename)) {
			throw std::exception();
		}
	
		// if the filesize is greater than the file threshold then put it in the big file queue
		if (fileSize >= FILE_THRESHOLD) {
			d_man_ptr->InsertBigFileRequest((size_t) fileSize, filename, conn);
		}
		else {
			d_man_ptr->InsertSmallFileRequest((size_t) fileSize, filename, conn);
		}
		
		conn->writeReply(reply_msg);
	}
	catch (std::exception &e) {
		// TODO	implement a mechanism to send an error message to say that the request is not recognized by the server 
		e.what();
	}

		 
}

bool RequestManager::_checkParameter(__int64 size, std::string filename) {

	if (size <= 0) {
		return false;
	}
	if (filename.empty()) {
		return false;
	}

	return true;

}


