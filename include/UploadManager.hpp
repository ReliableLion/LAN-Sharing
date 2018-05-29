#pragma once
#include <thread>
#include <vector>
#include "RequestHandler.hpp"


struct file_request;

class upload_manager {

	std::vector<std::thread> thread_pool_;

public:
	void upload_file(std::shared_ptr<file_request>);
	bool upload(std::shared_ptr<file_request>);

};
