#include "RequestManager.hpp"
#include "PacketManager.hpp"

RequestManager::RequestManager(std::shared_ptr<DownloadManager> download_manager) {
    is_terminated_.store(false);
    this->download_manager = download_manager;

    // instantiate all the threads that are used to manage all the incoming requests
    for (int i = 0; i < max_threads_; i++) {
        thread_pool_.push_back(std::thread(&RequestManager::extract_next_connection, this));
    }
}

/*
* this destructor close all the pending connections that are accepted by the server, but that are waiting a thread to elaborate the request
*/
RequestManager::~RequestManager() {
    terminate_service();
}


/**
* \brief extracts and close all pending connections, then join all the threads
*/
void RequestManager::terminate_service() {
    connection::conn_ptr connection;
    is_terminated_.store(true);                        // this flag is used to stop the threads computation

    while (!connection_queue_.isEmpty()) {            // iterate the entire queue and close the pending connections
        connection_queue_.popElement(connection);
        connection->close_connection();
    }

    for (auto i = 0; i < max_threads_; i++) {            // close all threads
        cv_.notify_all();
        thread_pool_[i].join();                        // "unlock" the threads that wait on the condition variable
    }

}

/**
* \brief insert the connection inside
* \param newConnection
* \return TRUE if the is possible to add the new connection into the queue, FALSE if isn't possible to add the connction either if the queue is full or the server should be closed
*/
bool RequestManager::add_connection(connection::conn_ptr newConnection, request_status &status) {
    // it is possible to add the connection into the queue of the requestManager only if the variable is terminated is true
    if (!is_terminated_.load()) {
        std::lock_guard<std::mutex> l(mtx_);

        if (!connection_queue_.insertElement(newConnection)) {
            status = FULL_QUEUE;
            return false;            // the queue has reached the max number of element
        }

        cv_.notify_all();
        return true;
    }

    status = TERM_SIGNAL;
    return false;                // this object has been closed
}

//		PRIVATE METHODS

void RequestManager::extract_next_connection() {
    connection::conn_ptr newConnection;
    std::unique_lock<std::mutex> ul(mtx_, std::defer_lock);
    bool exit = false;

    while (!exit) {
        ul.lock();

        cv_.wait(ul, [this]() {                                                        // wait on the condition varaible
            return (!connection_queue_.isEmpty() &&
                    !is_terminated_.load());            // unlock the condition variable only if the queue is not empty or the server has been closed
        });

        if (is_terminated_.load()) exit = true;                                        // if the server has been closed this method must return in order to join the threads

        // get the connection from the queue and then release the lock
        connection_queue_.popElement(newConnection);
        ul.unlock();
        download_request(newConnection);
    }
}

/**
* \brief process every pending request received by the server
* \param connection
*/
void RequestManager::download_request(connection::conn_ptr connection) {
    bool exit, received_correctly;
    int i = 0;

    PacketManager packet_manager(connection);

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

            // TODO lanciare un'eccezione per indicare che la connessione è stata chiusa
            ProtocolMessage packet = packet_manager.receive_packet();
            packet.compute_packet_type();

            switch (packet.get_message_code()) {
                case protocol::UNDEFINED : {
                    exit = false;
                    packet_manager.send_error(protocol::ERR_1);
                }
                    break;

                case protocol::ERR : {
                    exit = true;
                    // TODO aggiungere il dafarsi quando ricevo l'errore
                }
                    break;

                    // packet read correctly
                case protocol::OK : {
//                    if (process_request(packet_manager, connection)) {
//                        exit = true;
//                        received_correctly = true;
//                        packet_manager.send_reply(connection, MessageType::OK);
//                    } else {
//                        exit = false;
//                        i++;                                        // increment the bad-request counter
//                        std::cout << "impossible to recognize the request format " << std::endl;
//                        packet_manager.send_error(connection, MessageType::ERR_1);
//                    }


                    if (packet.compute_request()) {
                        if (validate_request(packet_manager, packet.get_message_request()))
                            received_correctly = true;
                        else
                            received_correctly = false;
                    } else {
                        // if the request is not valid send an error
                        packet_manager.send_error(protocol::ERR_1);
                        received_correctly = false;
                    }

                }
                    break;

                default:
                    break;
            }

        } while (!exit && i < max_request_attempts_);

        // check if the packet is received correctly
        if (received_correctly) {
            std::cout << "request received correclty" << std::endl;
        } else if (i == max_request_attempts_) {
            std::cout << "max attempts reached by the server, close the connection" << std::endl;
            connection->close_connection();
        }

    }
    catch (TimeoutException &te) {
        std::cout << "server reached the timeout, close the connection" << std::endl;
        packet_manager.send_error(protocol::ERR_1);
        connection->close_connection();
    }
    catch (SocketException &se) {
        std::cout << "server encourred in a socket exception, close the connection" << std::endl;
        packet_manager.send_error(protocol::ERR_1);
        connection->close_connection();
    }
}

bool RequestManager::validate_request(PacketManager &packet_manager, request_struct request) {
    //request_struct request = req_packet_manager.get_request();

    if (request.file_size <= 0) {
        packet_manager.send_error(protocol::ERR_1);
        return false;
    }
    if (request.file_name.length() > 256) {
        packet_manager.send_error(protocol::ERR_1);
        return false;
    }



}

bool RequestManager::forward_request(request_struct request, connection::conn_ptr connection) {

    // if is not possible to inset the data into the queue return an error
    if (request.file_size >= file_threshold_) {
        if (!download_manager->insert_big_file(request,connection)) return false;

        return true;
    } else {
        if (download_manager->insert_small_file(request, connection)) return false;

        return true;
    }
}


