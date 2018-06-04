#include "UploadManager.hpp"

void UploadManager::upload_file(std::shared_ptr<FileRequest> file_request) {

	std::thread thread;

	thread_pool_.push_back(std::thread(&UploadManager::upload, this, std::ref(file_request)));

}

bool UploadManager::upload(std::shared_ptr<FileRequest> file_request) {
	return true;
}
