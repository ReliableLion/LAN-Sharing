#include "UploadManager.hpp"
#include <MSWSock.h>

void UploadManager::upload_file(std::shared_ptr<FileRequest> file_request, WindowsFileHandler file_handler) {

	//std::thread thread;

	//thread_pool_.push_back(std::thread(&UploadManager::upload, this, std::ref(file_request)));

	upload(file_request, file_handler);
}

bool UploadManager::upload(std::shared_ptr<FileRequest> file_request, WindowsFileHandler file_handler) {

	//TransmitFile(file_request->connection_->get_handle_socket(), file_handler.get_file_handle(), file_request->file_size_, )

	return true;
}
