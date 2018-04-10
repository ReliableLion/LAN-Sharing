#include "WindowsFileHandler.hpp"

WindowsFileHandler::WindowsFileHandler(std::string path) : file_path_(path) {
	filename_ = get_file_name_from_full_path(file_path_);
}

WindowsFileHandler::~WindowsFileHandler() { close_file(); }

bool WindowsFileHandler::get_file_handle() {

	if ((file_handle_ = CreateFile(file_path_.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr)) != INVALID_HANDLE_VALUE)
		return true;

	return false;
}

void WindowsFileHandler::close_file() const {
	if(file_handle_ != INVALID_HANDLE_VALUE)
		CloseHandle(file_handle_);
}

void WindowsFileHandler::readFile(char *buffer, std::size_t size) {

}

DWORD WindowsFileHandler::get_file_size() const {
	if (file_handle_ != INVALID_HANDLE_VALUE)
		return GetFileSize(file_handle_, nullptr);
	return 0;
}

bool WindowsFileHandler::get_file_time(LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime) const {
	return GetFileTime(file_handle_, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
}

std::string WindowsFileHandler::getFilename() { return this->filename_; }

std::string WindowsFileHandler::getFilePath() {
	return file_path_;
}

std::string WindowsFileHandler::get_file_name_from_full_path(const std::string& file_path) {
	std::string file_name;

	const std::string::const_reverse_iterator it = std::find(file_path.rbegin(), file_path.rend(), '\\');

	if (it != file_path.rend()) {
		file_name.assign(file_path.rbegin(), it);
		std::reverse(file_name.begin(), file_name.end());
		return file_name;
	}
	else
		return file_name;
}
