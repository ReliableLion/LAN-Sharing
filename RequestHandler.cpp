#include "pch.h"

RequestHandler::RequestHandler() = default;

bool RequestHandler::send_request(const user_request destination_user, char* requestIDBuff, bool directory) {

	const auto port2 = DEFAULT_LISTEN_PORT;
	const auto file_path = destination_user.file_name;

	try {

		WindowsFileHandler file_handler(file_path);
		FILETIME write_time;

		string requestID = generate_random_string(30);
		strcpy_s(requestIDBuff, 31, requestID.c_str());
		std::cout << "REQUEST ID: " << requestID;

		if (!file_handler.open_file()) {
			throw WindowsFileHandlerException(std::string(file_path));
		}

		auto tcp_connection = std::make_shared<connection::TcpConnection>(destination_user.destination_user.client_address, port2);

		PacketDispatcher packet_dispatcher(tcp_connection);
	
		auto file_request = std::make_shared<FileRequest>();

		file_request->file_name_ = file_handler.get_file_path();
		file_request->requestID = requestID;
		file_request->destination_user = destination_user.destination_user;
		file_request->file_size_ = file_handler.get_file_size();
		file_request->connection_ = tcp_connection;
		file_request->directory = directory;

		std::cout << file_request->file_name_ << " DESTINAZIONE: " << file_request->destination_user.username << std::endl << "DIMENSIONE: " << file_request->file_size_ << std::endl;

		file_handler.get_file_time(nullptr, nullptr, &write_time);

		std::cout << file_handler.format_file_time(write_time) << std::endl;

		if (packet_dispatcher.send_packet(file_handler, directory)) {
			std::cout << "Packet sent! Now upload" << std::endl;

			fileRequests_.insert(std::make_pair(requestID, file_request));
			requests_.insert(std::make_pair(requestID, std::async(std::launch::async, &UploadManager::upload_file, file_request, std::move(file_handler), packet_dispatcher)));

			for (auto it = requests_.begin(); it != requests_.end(); ) {
			   if (it->second.wait_for(std::chrono::steady_clock::duration::zero()) == std::future_status::ready) {
				   auto save = it;
			       ++save;
			       requests_.erase(it);
			       it = save;
			   } else
			       ++it;
			}

			return true;
		}

		return false;

	} catch (SocketException &e) {
		std::cout << "Socket exception " << e.what() << std::endl;
		return false;
	} catch(WindowsFileHandlerException &e) {
		return false;
	}
}

void RequestHandler::cancel_request(string id) {
	
	for(auto it = fileRequests_.begin(); it != fileRequests_.end(); ++it) {
		if(it->first == id) {
			it->second->connection_->close_connection();
			break;
		}
	}
}


bool RequestHandler::is_terminated(string id) {
	
	auto result = requests_.end();

	for(auto it = requests_.begin(); it != requests_.end(); ++it) {
		if(it->first == id) {
			result = it;
			break;
		}
	}

	if (result != requests_.end()) {
	  return (result->second.wait_for(std::chrono::steady_clock::duration::zero())) == std::future_status::ready;
	}

	return false;
}

bool RequestHandler::get_result(string id) {

	//auto it = std::find_if(requests_.begin(), requests_.end(), [username, file_name](const std::pair<user_request, std::future<bool>>& obj) {return obj.first.destination_user.username == username & obj.first.file_name == file_name;});
	auto result = requests_.end();

	for(auto it = requests_.begin(); it != requests_.end(); ++it) {
		if(it->first == id) {
			result = it;
			break;
		}
	}

	if (result != requests_.end()) {
	  // found element. it is an iterator to the first matching element.
	  // if you really need the index, you can also get it:
	  if ((result->second.wait_for(std::chrono::steady_clock::duration::zero())) == std::future_status::ready) {

		  const auto result_code = result->second.get();
		  requests_.erase(result);

		  return result_code;
	  }
	}

	return false;
}
