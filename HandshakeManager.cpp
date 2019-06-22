#include "pch.h"

using namespace connection;

HandshakeManager::HandshakeManager(const std::shared_ptr<DownloadManager> download_manager, bool accept) {
    is_terminated_.store(false);
	auto_accept_.store(accept);
    this->download_manager_ = download_manager;

    // instantiate all the threads that are used to manage all the incoming requests
    for (auto i = 0; i < MAX_THREADS_; i++) {
        thread_pool_.push_back(std::thread(&HandshakeManager::extract_connections_from_queue, this, i));
    }
}

/*
 * This destructor close all the pending connections that are accepted by the server, but that are waiting a thread to elaborate the request
 */
HandshakeManager::~HandshakeManager() { terminate_service(); }

void HandshakeManager::terminate_service() {
    QueueConnectionElement element;
    is_terminated_.store(true);                        // this flag is used to stop the threads computation

    while (!connection_queue_.is_empty()) {            // iterate the entire queue and close the pending connections
        connection_queue_.pop_element(element);
        element.connection->close_connection();
    }

    for (auto i = 0; i < MAX_THREADS_; i++) {			// close all threads
        cv_.notify_all();
        thread_pool_[i].join();							// "unlock" threads that wait on the condition variable
    }
}

/*
 * This method add a connection to the queue of connections, return status code in case of error
 */
bool HandshakeManager::add_connection(const QueueConnectionElement element, request_status &status) {
    
    if (!is_terminated_.load()) {
        std::lock_guard<std::mutex> l(mtx_);

        if (!connection_queue_.insert_element(element)) {
            status = full_queue;
            return false;            // the queue has reached the max number of element
        }

        cv_.notify_all();
        return true;
    }

    status = term_signal;
    return false;
}

/*
 * Pass as argument to this function the identifier of the thread in order to do debugging
 */
void HandshakeManager::extract_connections_from_queue(int id) {
	QueueConnectionElement queue_element;
    std::unique_lock<std::mutex> ul(mtx_, std::defer_lock);

	int thread_id = id;

    while (true) {
        ul.lock();

        cv_.wait(ul, [this]() {															// wait on the condition varaible
			if (is_terminated_.load()) {
				return true;
			}
			else {
				return !connection_queue_.is_empty();
			}											// unlock the condition variable only if the queue is not empty or the server has been closed
        });

		if (is_terminated_.load())
			break;

		// get the connection from the queue and then release the lock
		connection_queue_.pop_element(queue_element);
		ul.unlock();

		PacketDispatcher packet_dispatcher(queue_element.connection);

		try {
			process_client_req(packet_dispatcher, queue_element.connection, thread_id);
		}
		catch (TimeoutException &te) {
			// send an error to the client
			// std::cout << "[Thread id: " << id << "]: Timeout exception" << std::endl;
			packet_dispatcher.send_packet(protocol::err_timeout);
			ConcurrentStreamPrint::print_data(thread_id, class_name, "Timeout exception, try to send an error message");
			requeue_connection(queue_element, thread_id, std::string("client doesn't respond"));
		}
		catch (PacketFormatException &pfe){
			// send an error to the client
			// std::cout << "[Thread id: " << id << "]: Packet exception" << std::endl;
			packet_dispatcher.send_packet(protocol::err_packet_format);
			ConcurrentStreamPrint::print_data(thread_id, class_name, "Packet exception, try to send and error message");
			requeue_connection(queue_element, thread_id, std::string("client responds with a wrong packet format"));
		}
    }
}

void HandshakeManager::requeue_connection(QueueConnectionElement element, int thread_id, std::string err_msg) {
	if (element.attempts < MAX_REQUEST_ATTEMPTS_) {
		request_status s;
		element.attempts++;
		add_connection(element, s);
	}
	else {
		ConcurrentStreamPrint::print_data(thread_id, class_name, "reached the maximum attempts, close the connection");
		element.connection->close_connection();		// close the connection if its reached the maximum of attempts
	}
}

void HandshakeManager::process_client_req(PacketDispatcher &packet_dispatcher, const connection_ptr connection, int thread_id) const {

    try {
		// std::cout << "[Thread id " << thread_id << "] try to receive the request" << std::endl;
		ConcurrentStreamPrint::print_data(thread_id, class_name, "try to receive the request");
		ProtocolMessage protocol_packet = packet_dispatcher.receive_packet();

		switch (protocol_packet.get_message_code()) {
			case protocol::ok: 
				// std::cout << "[Thread id " << thread_id << "]: received ok!" << std::endl;
				ConcurrentStreamPrint::print_data(thread_id, class_name, "received ok!");
				break;
			case protocol::send: {
				// std::cout << "[Thread id " << thread_id <<  "]: received send!" << std::endl;
				ConcurrentStreamPrint::print_data(thread_id, class_name, "received send!");
				request_struct request = protocol_packet.get_message_request();

				if (!auto_accept_.load()) {
					bool accept = managed_callback::getInstance().call_accept_callback(request.file_name_);
					if (!accept) {
						// return to the client the file error
						packet_dispatcher.send_packet(protocol::err_file);
						return;
					}
				}

				append_in_download_queue(packet_dispatcher, request, connection);
				packet_dispatcher.send_packet(protocol::err_1);
			}
				break;
			case protocol::err: 
				std::cout << "received error: "  <<  std::endl;
				break;
			case protocol::undefined: 
				// std::cerr << "[Thread id " << thread_id << "]: error, undefined packet type should be managed by an exception, bho" << std::endl;
				ConcurrentStreamPrint::print_data(thread_id, class_name, "error, undefined packet type should be managed by an exception, bho");
				break;
			default:
				break;
		}
    } 
	catch (SocketException &se) {
		// UNREFERENCED_PARAMETER(se);
		// std::cout << "[Thread id " << thread_id << "] " << class_name << ": Socket Exception, error code " << se.get_error_code() << std::endl;
		std::stringstream ss;
		ss << "Socket Exception, error code " << se.get_error_code();
		ConcurrentStreamPrint::print_data(thread_id, class_name, ss.str());
	} 
	catch (ConnectionCloseException &ce) {
		UNREFERENCED_PARAMETER(ce);
        // std::cout << "[Thread id " << thread_id << "] " << class_name << ": Connection close Exception" << std::endl;
		ConcurrentStreamPrint::print_data(thread_id, class_name, "Connection close Exception");
    }
}

/*
void HandshakeManager::decode_send_reqeust() {
	if (validate_request(packet_manager, req)) {

		std::cout << "sono qui " << std::endl;

		if (forward_request(req, connection)) {
			if (!packet_manager.send_packet(protocol::ok)) {
				std::cout << "the packet is not sent" << std::endl;
			}
		}
		else
			packet_manager.send_error(protocol::err_1);

	}
}

bool HandshakeManager::validate_request(PacketDispatcher &packet_fwd, request_struct request) {
    //request_struct request = req_packet_manager.get_request();

    // cambiare gli errori da inviare al client
    

	return true;
} */

void HandshakeManager::append_in_download_queue(PacketDispatcher &packet_dispatcher, const request_struct request, connection_ptr connection) const {

	if (request.file_size_ <= 0) 
		packet_dispatcher.send_packet(protocol::err_1);

	if (request.file_name_.length() > 256) 
		packet_dispatcher.send_packet(protocol::err_1);

    // if is not possible to inset the request into the queue return an error
    if (request.file_size_ >= FILE_THRESHOLD_) {
        if (!download_manager_->insert_big_file(request, connection)) 
			packet_dispatcher.send_packet(protocol::err_1);
    } else {
		if (!download_manager_->insert_small_file(request, connection))
			packet_dispatcher.send_packet(protocol::err_1);
	}

	packet_dispatcher.send_packet(protocol::ok);
}

void HandshakeManager::change_auto_accept(bool accept) {
	auto_accept_.store(accept);
}