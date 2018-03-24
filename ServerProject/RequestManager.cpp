#include "RequestManager.hpp"
#include "PacketManager.hpp"

//		PUBLIC METHODS

RequestManager::RequestManager(std::shared_ptr<DownloadManager> dwload_manager)
{
	is_terminated.store(false);
	this->dwload_manager = dwload_manager;

	// instantiate all the threads that are used to manage all the incoming requests
	for (int i = 0; i < maxThreads; i++)
	{
		threadPool.push_back(std::thread(&RequestManager::extract_next_connection, this));
	}
}

/*
 * this destructor close all the pending connections that are accepted by the server, but that are waiting a thread to elaborate the request
 */
RequestManager::~RequestManager() 
{
	terminateService();
}


/**
 * \brief extracts and close all pending connections, then join all the threads
 */
void RequestManager::terminateService()
{
	connection::conn_ptr connection;
	is_terminated.store(true);						// this flag is used to stop the threads computation

	while (!connectionQueue.isEmpty()) 
	{			// iterate the entire queue and close the pending connections
		connectionQueue.popElement(connection);
		connection->close_connection();
	}

	for (auto i = 0; i < maxThreads; i++)
	{			// close all threads
		cv.notify_all();
		threadPool[i].join();						// "unlock" the threads that wait on the condition variable
	}

}

/**
 * \brief insert the connection inside
 * \param newConnection 
 * \return TRUE if the is possible to add the new connection into the queue, FALSE if isn't possible to add the connction either if the queue is full or the server should be closed
 */
bool RequestManager::addConnection(connection::conn_ptr newConnection, request_status& status)
{
	// it is possible to add the connection into the queue of the requestManager only if the variable is terminated is true
	if (!is_terminated.load())
	{
		std::lock_guard<std::mutex> l(mtx);

		if (!connectionQueue.insertElement(newConnection)) 
		{
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

void RequestManager::extract_next_connection()
{
	connection::conn_ptr newConnection;
	std::unique_lock<std::mutex> ul(mtx, std::defer_lock);
	bool exit = false;

	while (!exit) 
	{
		ul.lock();

		cv.wait(ul, [this]() {														// wait on the condition varaible
			return (!connectionQueue.isEmpty() && !is_terminated.load());			// unlock the condition variable only if the queue is not empty or the server has been closed
		});
		
		if (is_terminated.load()) exit = true;										// if the server has been closed this method must return in order to join the threads
		
		// get the connection from the queue and then release the lock 
		connectionQueue.popElement(newConnection);	
		ul.unlock();
		receiveRequest(newConnection);
	}
}

/**
 * \brief process every pending request received by the server
 * \param connection 
 */
void RequestManager::receiveRequest(connection::conn_ptr connection) 
{
	bool exit, received_correctly;
	int i = 0;

	PacketManager packet_manager;

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

			switch (packet_manager.receivePacket(connection)) 
			{
				case CLSD_CONN:
				{
					// exit from the internal while but before close the connction
					exit = true;
					connection->close_connection();
					std::cout << "connection closed by peer" << std::endl;
				} break;

				// packet not recognized
				case PACKET_ERR:
				{							
					exit = false;
					i++;
					packet_manager.send_error();
				} break;

				// packet read correctly
				case READ_OK: 
				{							
					if (processRequest(packet_manager, connection)) 
					{
						exit = true;
						received_correctly = true;
						packet_manager.send_reply();
					} else 
					{
						exit = false;
						i++;										// increment the bad-request counter
						std::cout << "impossible to recognize the request format " << std::endl;
						packet_manager.send_error();
					}
				} break;

				default: 
				 break;
			}

		} while (!exit && i < max_request_attempts);

		// check if the packet is received correctly
		if (received_correctly) 
		{										
			std::cout << "request received correclty" << std::endl;
		} else if (i == max_request_attempts) 
		{
			packet_manager.send_error();
			connection->close_connection();
			std::cout << "max attempts reached by the server, close the connection" << std::endl;
		}

	}
	catch (TimeoutException &e) 
	{
		std::cout << "server reached the timeout, close the connection" << std::endl;
		packet_manager.send_error();
		connection->close_connection();
	}
	catch (SocketException &e)
	{
		std::cout << "server encourred in a socket exception, close the connection" << std::endl;
		packet_manager.send_error();
		connection->close_connection();
	}
}


/**
 * \brief return false only if there is a packet error, if there is a server error (service terminated) return true
 * \param req_packet_manager 
 * \param connection 
 * \return 
 */
bool RequestManager::processRequest(PacketManager& req_packet_manager, connection::conn_ptr connection)
{
	request_struct request = req_packet_manager.get_request();

	if (request.file_size <= 0)
	{
		req_packet_manager.send_error();
		return false;
	}
	if (request.file_name.length() > 256)
	{
		req_packet_manager.send_error();
		return false;
	}
								
	// if is not possible to inset the data into the queue return an error
	if (request.file_size >= fileThreshold)
	{
		if(!dwload_manager->insertBigFile(request, connection))
		{
			req_packet_manager.send_error();
		}
	} else
	{
		if(dwload_manager->insertSmallFile(request, connection))
		{
			req_packet_manager.send_error();
		}
	}

	return true;
}


