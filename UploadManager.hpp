#pragma once
#include <thread>
#include <vector>
#include "RequestHandler.hpp"
#include "WindowsFileHandler.hpp"
#include "PacketDispatcher.hpp"

struct FileRequest;

class UploadManager {

public:
	static bool upload_file(std::shared_ptr<FileRequest>, WindowsFileHandler, PacketDispatcher);
};
