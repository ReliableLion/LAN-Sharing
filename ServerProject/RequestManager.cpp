#include "RequestManager.hpp"
#include "PacketManager.hpp"

//		PUBLIC METHODS

RequestManager::RequestManager() : download_manager() {
	is_terminated.store(false);
	for (int i = 0; i < maxThreads; i++) {
		threadPool.push_back(std::thread(&RequestManager::extractConnection, this));
	}
}

RequestManager::~RequestManager() {
	is_terminated.store(true);						// this flag is used to stop the threads computation
	session::conn_ptr connection;		

	while (!connectionQueue.isEmpty()) {			// iterate the entire queue and close the pending connections
		connectionQueue.popElement(connection);
		connection->close_connection();
	}

	for (auto i = 0; i < maxThreads; i++) {			// close all the threads
		cv.notify_all();
		threadPool[i].join();						// "unlock" the threads that wait on the condition variable
	}
}

/**
 * \brief insert the connection inside
 * \param newConnection 
 * \return 
 */
bool RequestManager::addConnection(session::conn_ptr newConnection) {
	if (!is_terminated.load()) {

		std::lock_guard<std::mutex> l(mtx);
		if (!connectionQueue.insertElement(newConnection)) {
			return false;			// the queue has reached the max number of element
		}
		return true;
	}
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
		connectionQueue.popElement(newConnection);	
		ul.unlock();

		receiveRequest(newConnection);
	}
}

/**
 * \brief process every pending request received by the server
 * \param connection 
 */
void RequestManager::receiveRequest(session::conn_ptr connection) {
	PacketManager packet_manager;
	bool received_correctly;
	bool connection_closed;
	int i = 0;

	// TODO declare here the requestManager and ReplyManager
	try {
		do {
			received_correctly = false;
			connection_closed = false;
			/*
			-if the packet is received correctly, write the reply and set received correclty true
				-if the request is accepted pass the connection to the downloadmanager
				-if the requets is not accepted send an error
			-if the packet is not received correctly, send an error and set the index + 1
			-if the connection is closed, write a message
			*/

			switch (packet_manager.receivePacket(connection)) {
				case (CLD_CONN): {									// connection closed
					connection_closed = true;
				} break;

				case (URZ_PACKET): {								// packet not recognized
				
					received_correctly = false;
					i++;
					// send an error 

				} break;

				case (READ_CORRECTLY): {							// packet read correctly

					if (processRequest(packet_manager, connection)) {
						received_correctly = false;
						i++;										// increment the bad-request counter
					} else {
						received_correctly = true;
					}
				
				} break;

				default: {
				} break;
			}

		} while (received_correctly || connection_closed || i == maxRequestAttempts);

		// check if the pac
		if (!received_correctly) {										
			std::cout << "request received correclty" << std::endl;
		} else 
		if (i == maxRequestAttempts) {
			// TODO close the connection
			connection->close_connection();
			std::cout << "max attempts reached by the server, close the connection" << std::endl;
		} else 
		if (connection_closed) {
			std::cout << "connection closed by peer" << std::endl;
		}
		
	}
	catch (TimeoutException &e) {
		// TODO close the connection 
		connection->close_connection();
	}
	catch (SocketException &e) {
		// TODO close the connection
		connection->close_connection();
	}
}

bool RequestManager::sendReply(session::conn_ptr conn) {
	return true;
}

bool RequestManager::processRequest(PacketManager& packet_manager, session::conn_ptr connection) {
	requestStruct request = packet_manager.get_request_struct();

	if (!packet_manager.checkRequest()) return false;
	
	if (request.fileSize >= fileThreshold) {
		download_manager.insertBigFile(request, connection);
	} else {
		download_manager.insertSmallFile(request, connection);
	}

	return true;
}


