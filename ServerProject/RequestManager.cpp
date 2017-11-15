/*#include "RequestMananger.h"

// when a new object ReqeuestManager is instantiated the Server pass as reference the DownloadManager
RequestManager::RequestManager(std::shared_ptr<DownloadManager> d_man_ptr) {
	this->d_man_ptr = d_man_ptr;
	terminate.store(false);

	for (int i = 0; i < MAX_THREADS; i++) {
		t[i] = std::thread();
	}

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

// close connection should be called after the shutdown
void RequestManager::closeConnections() {
	conn_ptr conn;
	// if shutdown method is called then all the connections that are in connection_pool are closed
	while (connection_pool.isEmpty()) {
		connection_pool.popElement(conn);
		conn->closeConnection();
	}
}

bool RequestManager::addRequest(conn_ptr conn) {
	// check if the server is closed
	if (terminate.load()) {
		return false;
	}
	// add the request into the queue
	std::lock_guard <std::mutex> l(mtx1);
	connection_pool.insertElement(conn);
	cv.notify_all();
	return true;
}

void RequestManager::_processRequest() {
	try {
		conn_ptr conn;
		while (1) {
			std::unique_lock<std::mutex> l(mtx1);
			cv.wait(l, [this]() {
				return (!connection_pool.isEmpty() || terminate.load());
			});

			if (terminate.load()) { break; }
			connection_pool.popElement(conn);
			l.unlock();

			// here we receive the client request and decode it in order to begin the file download
			_requestHandShake(conn);
		}
	}
	catch (std::exception &e) {

	}
}

// with handshake i mean the time window between the request and the reply
void RequestManager::_requestHandShake(conn_ptr conn) {
	RequestMessage r_msg;
	ReplyMessage reply_msg;
	__int64 fileSize = 0;
	std::string filename = "";

	// TODO	implement a timeout mechanism in order to close connections that are open but unused
	// check if the connection is alive
	if (!conn->checkConnection()) { 
		return;
	}

	try {
		conn->readRequest(r_msg);

		/*				
		*	check if the request is valid or not
		*/

		//TODO	devo aspettare davide che finisca la class message
		// TODO	i need to use the message class methods to decode the content of the request

		/*if (_checkParameter(fileSize, filename)) {
			std::cout << "filename is null or the filesize is less than 0" << std::endl;
			return;
		}

		/*
		*	insert the request in the right queue
		*/
		/*try {
			if (fileSize >= FILE_THRESHOLD) {
				d_man_ptr->InsertBigFileRequest((size_t)fileSize, filename, conn);
			}
			else {
				d_man_ptr->InsertSmallFileRequest((size_t)fileSize, filename, conn);
			}
		}
		catch (RequestInsertionException &e) {
			// is not possible to insert in the queue the request
			std::cout << e.what() << std::endl;
			return;
		}
		
		/*
		*	send a reply message to the client
		*/
		/*conn->writeReply(reply_msg);
	}
	catch (std::exception &e) {
		// TODO	implement a mechanism to send an error message to say that the request is not recognized by the server 
		std::cout << e.what() << std::endl;
	}
	/*
	 manage the case with that is impossible to send the request to the download manager
	*/	 
/*}

bool RequestManager::_checkParameter(__int64 size, std::string filename) {
	if (size <= 0) {
		return false;
	}
	if (filename.empty()) {
		return false;
	}
	return true;
}*/


