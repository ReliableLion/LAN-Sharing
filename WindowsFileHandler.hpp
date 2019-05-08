#pragma once

#include <string>

class WindowsFileHandler {

protected:
	std::string file_dir_;								// TODO remember to write \\ instead of \ when you write the file path
	std::string filename_;
	std::string file_path_;
	HANDLE file_handle_ = INVALID_HANDLE_VALUE;

	static std::string get_file_name_from_full_path(const std::string& file_path);
	WindowsFileHandler(const WindowsFileHandler& source) {}

public:
	explicit WindowsFileHandler(std::string path);

	~WindowsFileHandler();

	WindowsFileHandler::WindowsFileHandler(WindowsFileHandler&& source) noexcept {
		this->file_handle_ = source.file_handle_;
		this->filename_ = source.filename_;
		this->file_dir_ = source.file_dir_;
		this->file_path_ = source.file_path_;

		source.file_handle_ = INVALID_HANDLE_VALUE;
	}

	WindowsFileHandler& operator=(WindowsFileHandler&& source) noexcept {
		if (this != &source) {
			this->file_handle_ = source.file_handle_;
			this->filename_ = source.filename_;
			this->file_dir_ = source.file_dir_;
			this->file_path_ = source.file_path_;

			source.file_handle_ = INVALID_HANDLE_VALUE;
		}

		return *this;
	}

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
