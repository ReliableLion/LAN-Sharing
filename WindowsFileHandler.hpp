#pragma once

#include <string>
#include "stdafx.h"

class WindowsFileHandler {

protected:
	std::string file_dir_;								// TODO remember to write \\ instead of \ when you write the file path
	std::string filename_;
	std::string file_path_;
	HANDLE file_handle_ = INVALID_HANDLE_VALUE;

	static std::string get_file_name_from_full_path(const std::string& file_path);

public:
	explicit WindowsFileHandler(std::string path);

	~WindowsFileHandler();

	bool open_file();

	void close_file() const;

	static void read_file(char *buffer, std::size_t size);

	DWORD get_file_size() const;

	bool get_file_time(_Out_opt_ LPFILETIME lp_creation_time, _Out_opt_ LPFILETIME lp_last_access_time, _Out_opt_ LPFILETIME lp_last_write_time) const;

	static std::string format_file_time(FILETIME);

	std::string get_filename() const;

	std::string get_file_path() const;

	HANDLE get_file_handle() const { return this->file_handle_;  }
};
