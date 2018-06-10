#include "UploadManager.hpp"
#include <iostream>
#include "Exceptions.hpp"

#pragma comment(lib, "Mswsock.lib")

void UploadManager::upload_file(std::shared_ptr<FileRequest> file_request, WindowsFileHandler file_handler, PacketManager packet_manager) {

	//std::thread thread;

	//thread_pool_.push_back(std::thread(&UploadManager::upload, this, std::ref(file_request)));
	ProtocolMessage packet_;

	packet_.replace(packet_manager.receive_packet());
	packet_.compute_packet_type();

	switch(packet_.get_message_code()) {
		case protocol::ok: {
			std::cout << "File sending..." << std::endl;
			if(upload(file_request, file_handler.get_file_handle()))
				file_request->transferred_bytes_ = file_request->file_size_;
			else {
				file_request->transferred_bytes_ = 0;
			}

			break;
		}
		case protocol::err: {
			file_request->transferred_bytes_ = 0;
			break;
		}
		default:
			throw FileTransmissionException(1);
	}

	std::cout << "Aspetto di ricevere risposta!" << std::endl;

	packet_.replace(packet_manager.receive_packet());
	packet_.compute_packet_type();

	std::cout << "RICEVUTO: " << packet_.get_message_code() << std::endl;

	if (packet_.get_message_code() == protocol::ok) {
		std::cout << "File sent!" << std::endl;
		file_request->connection_->close_connection();
	}
	else {
		std::cout << "File not received! Here the response: " << packet_.get_message_code() << std::endl;
		file_request->connection_->close_connection();
	}
}

bool UploadManager::upload(std::shared_ptr<FileRequest> file_request, HANDLE file_handle) {

	if (!TransmitFile(file_request->connection_->get_handle_socket(), file_handle, file_request->file_size_, 0, nullptr, nullptr, TF_USE_KERNEL_APC)) {
		std::cout << "ERROR TRANSMIT FILE: " << WSAGetLastError() << std::endl;
		return false;
	}

	return true;
}


