#pragma once
#include <thread>
#include <vector>
#include "RequestHandler.hpp"


struct FileRequest;

class UploadManager {

	std::vector<std::thread> thread_pool_;

public:
	void upload_file(std::shared_ptr<FileRequest>);
	bool upload(std::shared_ptr<FileRequest>);
};
