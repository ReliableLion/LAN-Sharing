#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include "Exceptions.hpp"
#include "Constants.hpp"
#include "stdafx.h"

class WindowsFileHandler {

protected:
	std::string file_dir_;								// TODO remember to write \\ instead of \ when you write the file path
	std::string filename_;
	std::string file_path_;
	HANDLE file_handle_ = INVALID_HANDLE_VALUE;

	std::string WindowsFileHandler::get_file_name_from_full_path(const std::string& file_path);

public:
	WindowsFileHandler(std::string path);
	~WindowsFileHandler();
	bool get_file_handle();
	void close_file() const;
	void readFile(char *buffer, std::size_t size);
	DWORD get_file_size() const;
	bool get_file_time(_Out_opt_ LPFILETIME lpCreationTime, _Out_opt_ LPFILETIME lpLastAccessTime, _Out_opt_ LPFILETIME lpLastWriteTime) const;
	std::string getFilename();
	std::string getFilePath();
};
