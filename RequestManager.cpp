#include "RequestManager.hpp"
#include <iostream>
#include "Exceptions.hpp"

RequestManager::RequestManager(const std::shared_ptr<DownloadManager> download_manager) {
    is_terminated_.store(false);
    this->download_manager_ = download_manager;

    // instantiate all the threads that are used to manage all the incoming requests
    for (auto i = 0; i < MAX_THREADS_; i++) {
        thread_pool_.push_back(std::thread(&RequestManager::extract_next_connection, this));
    }
}

/*
* this destructor close all the pending connections that are accepted by the server, but that are waiting a thread to elaborate the request
*/
RequestManager::~RequestManager() { terminate_service(); }

void RequestManager::terminate_service() {
    connection::conn_ptr connection;
    is_terminated_.store(true);                        // this flag is used to stop the threads computation

    while (!connection_queue_.is_empty()) {            // iterate the entire queue and close the pending connections
        connection_queue_.pop_element(connection);
        connection->close_connection();
    }

    for (auto i = 0; i < MAX_THREADS_; i++) {            // close all threads
        cv_.notify_all();
        thread_pool_[i].join();                        // "unlock" the threads that wait on the condition variable
    }

}

bool RequestManager::add_connection(const connection::conn_ptr new_connection, request_status &status) {
    // it is possible to add the connection into the queue of the requestManager
    // only if the variable is_terminated is true
    if (!is_terminated_.load()) {
        std::lock_guard<std::mutex> l(mtx_);

        if (!connection_queue_.insert_element(new_connection)) {
            status = full_queue;
            return false;            // the queue has reached the max number of element
        }

        cv_.notify_all();
        return true;
    }

    status = term_signal;
    return false;                // this object has been closed
}

void RequestManager::extract_next_connection() {
    connection::conn_ptr connection;
    std::unique_lock<std::mutex> ul(mtx_, std::defer_lock);
	auto exit = false;

    while (!exit) {
        ul.lock();

        cv_.wait(ul, [this]() {                                                        // wait on the condition varaible
            return (!connection_queue_.is_empty() &&
                    !is_terminated_.load());            // unlock the condition variable only if the queue is not empty or the server has been closed
        });

        if (is_terminated_.load())
            exit = true;                                        // if the server has been closed this method must return in order to join the threads
        else {
            // get the connection from the queue and then release the lock
            connection_queue_.pop_element(connection);
            ul.unlock();
            download_request(connection);
        }
    }
}

void RequestManager::download_request(const connection::conn_ptr connection) const {
	auto exit = false, received_correctly = false;
	auto i = 0;

    PacketManager packet_manager(connection);

    // TODO declare here the requestManager and ReplyManager
    try {
        while (!exit && i++ < MAX_REQUEST_ATTEMPTS_) {

            /*
            -if the packet is received correctly, write the reply and set received correclty true
            -if the request is accepted pass the connection to the downloadmanager
            -if the requets is not accepted send an error
            -if the packet is not received correctly, send an error and set the index + 1
            -if the connection is closed, write a message
            */

	        auto packet = packet_manager.receive_packet();
            packet.compute_packet_type();

            switch (packet.get_message_code()) {

                case protocol::undefined : {
                    packet_manager.send_error(protocol::err_1);
                    exit = false;
                }
                    break;

				case protocol::ok : {
					std::cout << "received ok!" << std::endl;
				}
				break;

				case protocol::send: {
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
                   if (packet.compute_send_request()) {
	                   const auto req = packet.get_message_request();

                       if (validate_request(packet_manager, req)) {

                          if (forward_request(req, connection)) {
                              exit = true;
                          } else {
                              packet_manager.send_error(protocol::err_1);
                              exit = false;
                          }

                       }
                       else exit = false;
                   } else {
                        // if the request is not valid send an error
                       packet_manager.send_error(protocol::err_1);
                       exit = false;
                   }

                }
                    break;

                default:
                    break;
            }

        }

        // check if the packet is received correctly
        if (!exit && i > MAX_REQUEST_ATTEMPTS_) {
            std::cout << "max attempts reached by the server, close the connection" << std::endl;
            connection->close_connection();
        }

        std::cout << "request received correclty" << std::endl;

    } catch (TimeoutException &te) {
		UNREFERENCED_PARAMETER(te);
        std::cout << "server reached the timeout, close the connection" << std::endl;
        packet_manager.send_error(protocol::err_1);
        connection->close_connection();
    } catch (SocketException &se) {
		UNREFERENCED_PARAMETER(se);
        std::cout << "server encourred in a socket exception, close the connection" << std::endl;
        packet_manager.send_error(protocol::err_1);
        connection->close_connection();
    } catch (ConnectionCloseException &ce) {
		UNREFERENCED_PARAMETER(ce);
        std::cout << "the connection has been closed by the peer" << std::endl;
    }
}

bool RequestManager::validate_request(PacketManager &packet_manager, request_struct request) {
    //request_struct request = req_packet_manager.get_request();

    //  TODO cambiare gli errori da inviare al client
    if (request.file_size_ <= 0) {
        packet_manager.send_error(protocol::err_1);
        return false;
    }
    if (request.file_name_.length() > 256) {
        packet_manager.send_error(protocol::err_1);
        return false;
    }
	return true;
}

bool RequestManager::forward_request(const request_struct request, connection::conn_ptr connection) const {

    // if is not possible to inset the request into the queue return an error
    if (request.file_size_ >= FILE_THRESHOLD_) {
        if (!download_manager_->insert_big_file(request, connection)) return false;

        std::cout << "the request is forwarded to the big file queue" << std::endl;
        return true;
    } else {
       if (!download_manager_->insert_small_file(request, connection)) return false;

        std::cout << "the request is forwarded to the small file queue"  << std::endl;
        return true;
    }
}


