#include "RequestManager.hpp"
#include "PacketManager.hpp"

//		PUBLIC METHODS

RequestManager::RequestManager() : download_manager() {
	is_terminated.store(false);

	// instantiate all the threads that are used to manage the request that the server receives
	for (int i = 0; i < maxThreads; i++) {
		threadPool.push_back(std::thread(&RequestManager::extractConnection, this));
	}
}

/*
 * this destructor close all the pending connections that are accepted by the server, but that are waiting a thread to elaborate the request
 */
RequestManager::~RequestManager() {
	is_terminated.store(true);						// this flag is used to stop the threads computation
	session::conn_ptr connection;		

	while (!connectionQueue.isEmpty()) {			// iterate the entire queue and close the pending connections
		connectionQueue.popElement(connection);
		connection->close_connection();
	}

	for (auto i = 0; i < maxThreads; i++) {			// close all threads
		cv.notify_all();
		threadPool[i].join();						// "unlock" the threads that wait on the condition variable
	}
}

/**
 * \brief insert the connection inside
 * \param newConnection 
 * \return TRUE if the is possible to add the new connection into the queue, FALSE if isn't possible to add the connction either if the queue is full or the server should be closed
 */
bool RequestManager::addConnection(session::conn_ptr newConnection, request_status& status) {
	if (!is_terminated.load()) {
		std::lock_guard<std::mutex> l(mtx);

		if (!connectionQueue.insertElement(newConnection)) {
			status = FULL_QUEUE;
			return false;			// the queue has reached the max number of element
		}

		cv.notify_all();
		return true;
	}
	
	status = TERM_SIGNAL;
	return false;				// this object has been closed
}

//		PRIVATE METHODS

void RequestManager::extractConnection() {
	session::conn_ptr newConnection;
	std::unique_lock<std::mutex> ul(mtx, std::defer_lock);

	while (true) {
		ul.lock();

		cv.wait(ul, [this]() {														// wait on the condition varaible
			return (!connectionQueue.isEmpty() || is_terminated.load());			// unlock the condition variable only if the queue is not empty or the server has been closed
		});
		
		if (is_terminated.load()) break;											// if the server has been closed this method must return in order to join the threads
		
		// get the connection from the queue and then release the lock 
		connectionQueue.popElement(newConnection);	
		ul.unlock();

		// permorm the operation to the new connection that has been extracted from the queue
		receiveRequest(newConnection);
	}
}

/**
 * \brief process every pending request received by the server
 * \param connection 
 */
void RequestManager::receiveRequest(session::conn_ptr connection) {
	PacketManager packet_manager;
	bool exit, received_correctly;
	int i = 0;

	// TODO declare here the requestManager and ReplyManager
	try {
		do {
			exit = false;
			received_correctly = false;

			/*
			-if the packet is received correctly, write the reply and set received correclty true
				-if the request is accepted pass the connection to the downloadmanager
				-if the requets is not accepted send an error
			-if the packet is not received correctly, send an error and set the index + 1
			-if the connection is closed, write a message
			*/

			switch (packet_manager.receivePacket(connection)) {
				
				// connection closed
				case (CLSD_CONN): {
					// exit from the internal while but before close the connction
					exit = true;
					connection->close_connection();
					std::cout << "connection closed by peer" << std::endl;
				} break;

				// packet not recognized
				case (PACKET_ERR): {							
					exit = false;
					i++;
					// TODO send an error
				} break;

				// packet read correctly
				case (READ_OK): {							

					if (processRequest(packet_manager, connection)) {
						exit = true;
						received_correctly = true;
						// TODO send an error
					} else {
						exit = false;
						i++;										// increment the bad-request counter

						//std::cout << "impossible to recognize the request format " << std::endl;

						// TODO send an error
					}
					
				} break;

				default: 
				 break;
			}

		} while (!exit && i < max_request_attempts);

		// check if the packet is received correctly
		if (received_correctly) {										
			std::cout << "request received correclty" << std::endl;
		} else
		
		// close the connection if the server reach the max connection attempts
		if (i == max_request_attempts) {
			connection->close_connection();
			std::cout << "max attempts reached by the server, close the connection" << std::endl;
		}

	}
	catch (TimeoutException &e) {
		std::cout << "server reached the timeout, close the connection" << std::endl;
		connection->close_connection();
	}
	catch (SocketException &e) {
		std::cout << "server encourred in a socket exception, close the connection" << std::endl;
		connection->close_connection();
	}
}

bool RequestManager::sendReply(session::conn_ptr connection) {
	return true;
}

bool RequestManager::processRequest(PacketManager& packet_manager, session::conn_ptr connection) {
	request_struct request = packet_manager.get_request_struct();

	if (!packet_manager.check_request()) return false;
	
	if (request.file_size >= fileThreshold) {
		download_manager.insertBigFile(request, connection);
	} else {
		download_manager.insertSmallFile(request, connection);
	}

	return true;
}


