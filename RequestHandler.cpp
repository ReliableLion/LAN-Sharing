#include "RequestHandler.hpp"

request_handler::request_handler(const std::shared_ptr<upload_manager> upload_manager) {
	this->upload_manager_ = upload_manager;
}

bool request_handler::send_request(char* server, char *file_path) {

	connection::TCPConnection tcp_connection;
	
	std::string port = std::to_string(DEFAULT_LISTEN_PORT);
	if (tcp_connection.create_connection(server, port.c_str())) {

		requests_.insert(std::make_pair(get_file_name_from_full_path(file_path),std::make_shared<connection::TCPConnection>(tcp_connection)));

		// TODO call upload manager to handle the request in another thread

		return true;
	}

	return false;
}

std::string request_handler::get_file_name_from_full_path(const std::string& file_path) {
	std::string file_name;

	const std::string::const_reverse_iterator it = std::find(file_path.rbegin(), file_path.rend(), '\\');

	if (it != file_path.rend()) {
		file_name.assign(file_path.rbegin(), it);
		std::reverse(file_name.begin(), file_name.end());
		return file_name;
	}
	else
		return file_name;
}