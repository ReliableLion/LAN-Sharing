#include "UploadManager.hpp"
#include <iostream>
//#include <stdlib.h>     //for using the function sleep

#pragma comment(lib, "Mswsock.lib")

bool UploadManager::upload_file(std::shared_ptr<FileRequest> file_request, WindowsFileHandler file_handler, PacketDispatcher packet_dispatcher) {

	//std::thread thread;

	//thread_pool_.push_back(std::thread(&UploadManager::upload, this, std::ref(file_request)));
	std::cout << "Waiting for packet" << std::endl;
	ProtocolMessage packet = packet_dispatcher.receive_packet();
	packet.compute_packet_type();

	if (packet.get_message_code() == protocol::ok) {
		std::cout << "File sending..." << std::endl;

		if(!TransmitFile(file_request->connection_->get_handle_socket(), file_handler.get_file_handle(), file_request->file_size_, 0, nullptr, nullptr, TF_USE_KERNEL_APC)) {
			std::cout << "ERROR TRANSMIT FILE: " << WSAGetLastError() << std::endl;
			file_request->connection_->close_connection();
			return false;
		}

		std::cout << "Aspetto di ricevere OK!" << std::endl;

		ProtocolMessage packet2 = packet_dispatcher.receive_packet();
		packet2.compute_packet_type();

		std::cout << "RICEVUTO: " << packet2.get_message_code() << std::endl;

/*
		std::cout << "VADO IN SLEEP" << std::endl;
		Sleep(20000);*/

		std::cout << "File sent!" << std::endl;
		file_request->connection_->close_connection();

		return true;
	}

	std::cout << "The message wasn't ok" << std::endl;

	return false;
}
