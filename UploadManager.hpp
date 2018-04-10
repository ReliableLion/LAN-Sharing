#pragma once

#include "RequestHandler.hpp"

#include <map>
#include <thread>

struct file_request;

class upload_manager {

	std::map<std::shared_ptr<file_request>,std::thread> thread_pool_;

public:
	void upload_file(std::shared_ptr<file_request>);

};
