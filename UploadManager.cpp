#include "UploadManager.hpp"

void upload_manager::upload_file(std::shared_ptr<file_request> file_request) {

	std::thread thread;

	thread_pool_.push_back(std::thread(&upload_manager::upload, this, std::ref(file_request)));

}

bool upload_manager::upload(std::shared_ptr<file_request> file_request) {
	return true;
}
