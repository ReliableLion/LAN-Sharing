#include "UploadManager.hpp"
#include <iostream>
#include <stdlib.h>     //for using the function sleep

#pragma comment(lib, "Mswsock.lib")

void UploadManager::upload_file(std::shared_ptr<FileRequest> file_request, WindowsFileHandler file_handler, PacketManager packet_manager) {

	//std::thread thread;

	//thread_pool_.push_back(std::thread(&UploadManager::upload, this, std::ref(file_request)));

	ProtocolMessage packet = packet_manager.receive_packet();
	packet.compute_packet_type();

	if (packet.get_message_code() == protocol::ok) {
		std::cout << "File sending..." << std::endl;

		if(!TransmitFile(file_request->connection_->get_handle_socket(), file_handler.get_file_handle(), file_request->file_size_, 65535, nullptr, nullptr, TF_USE_SYSTEM_THREAD)) {
			std::cout << "ERROR TRANSMIT FILE: " << WSAGetLastError() << std::endl;
			file_request->connection_->close_connection();
			return;
		}

		std::cout << "VADO IN SLEEP" << std::endl;
		Sleep(5000);

		std::cout << "File sent!" << std::endl;
		file_request->connection_->close_connection();

		return;
	}

	std::cout << "The message wasn't ok" << std::endl;
}
