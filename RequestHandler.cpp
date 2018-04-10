#include "RequestHandler.hpp"
#include "FileHandler.hpp"
#include "WindowsFileHandler.hpp"

request_handler::request_handler(const std::shared_ptr<upload_manager> upload_manager) {
	this->upload_manager_ = upload_manager;
}

bool request_handler::send_request(char* server, char *file_path) {

	connection::TCPConnection tcp_connection;

	WindowsFileHandler file_handler(file_path);
	
	if (!file_handler.get_file_handle())
		return false;
	
	std::string port = std::to_string(DEFAULT_LISTEN_PORT);

	if (tcp_connection.create_connection(server, port.c_str())) {

		file_request file_request;
		file_request.file_name = file_handler.getFilePath();
		file_request.destination_address = server;
		file_request.file_size = file_handler.get_file_size();
		file_request.connection = std::make_shared<connection::TCPConnection>(tcp_connection);

		requests_.push_back(file_request);

		if (packet_manager_.send_packet(std::make_shared<connection::TCPConnection>(tcp_connection), file_handler) == READ_OK) {
			upload_manager_->upload_file(std::make_shared<::file_request>(file_request));
			return true;
		}
		// TODO call upload manager to handle the request in another thread

		return false;
	}

	return false;
}
