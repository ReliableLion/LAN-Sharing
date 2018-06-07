#include "UploadManager.hpp"
#include <iostream>

#pragma comment(lib, "Mswsock.lib")

void UploadManager::upload_file(std::shared_ptr<FileRequest> file_request, WindowsFileHandler file_handler, PacketManager packet_manager) {

	//std::thread thread;

	//thread_pool_.push_back(std::thread(&UploadManager::upload, this, std::ref(file_request)));

	ProtocolMessage packet = packet_manager.receive_packet();
	packet.compute_packet_type();

	if(packet.get_message_code() == protocol::ok)
		upload(file_request, file_handler);

	std::cout << "The message wasn't ok" << std::endl;
}

bool UploadManager::upload(std::shared_ptr<FileRequest> file_request, WindowsFileHandler file_handler) {

	// Todo check in future lpOverlapped parameter which is set to 0 for now. Should be right because the file is opened as non overlapped
	TransmitFile(file_request->connection_->get_handle_socket(), file_handler.get_file_handle(), file_request->file_size_, 0, nullptr, nullptr, TF_USE_SYSTEM_THREAD);

	return true;
}
