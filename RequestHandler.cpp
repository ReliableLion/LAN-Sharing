#include "RequestHandler.hpp"
#include "FileHandler.hpp"
#include "WindowsFileHandler.hpp"
#include <iostream>
#include "Exceptions.hpp"

RequestHandler::RequestHandler(const std::shared_ptr<UploadManager> upload_manager) {
	this->upload_manager_ = upload_manager;
}

bool RequestHandler::send_request(const std::string server, char *file_path) {

	const auto port2 = DEFAULT_LISTEN_PORT;

	connection::TcpConnection tcp_connection(server, port2);

	WindowsFileHandler file_handler(file_path);
	
	if (!file_handler.open_file())
		throw WindowsFileHandlerException(std::string(file_path));
	
	//std::string port = std::to_string(DEFAULT_LISTEN_PORT);

	//if (tcp_connection.create_connection(server, port.c_str())) {

		FileRequest file_request;
		file_request.file_name_ = file_handler.get_file_path();
		file_request.destination_address_ = server;
		file_request.file_size_ = file_handler.get_file_size();
		file_request.connection_ = std::make_shared<connection::TcpConnection>(tcp_connection);

		std::cout << file_request.file_name_ << " DESTINAZIONE: " << file_request.destination_address_ << std::endl << "DIMENSIONE: " << file_request.file_size_ << std::endl;

		FILETIME write_time;
		file_handler.get_file_time(nullptr, nullptr, &write_time);

		std::cout << file_handler.format_file_time(write_time) << std::endl;

		requests_.push_back(file_request);

		// TODO packet manager must be changed and initialized here every time
		PacketManager packet_manager(std::make_shared<connection::TcpConnection>(tcp_connection));

		if (packet_manager.send_packet(file_handler)) {
			std::cout << "Packet sent! Now upload" << std::endl;
			upload_manager_->upload_file(std::make_shared<FileRequest>(file_request), std::move(file_handler), packet_manager);
			return true;
		}
		// TODO call upload manager to handle the request in another thread

		return false;
	//}

	//return false;
}
