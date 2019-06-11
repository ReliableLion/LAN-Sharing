#pragma once
#include "pch.h"

struct FileRequest;

class UploadManager {

public:
	static bool upload_file(std::shared_ptr<FileRequest>, WindowsFileHandler, PacketDispatcher);
};
