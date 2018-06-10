#include "WindowsFileHandler.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm>

WindowsFileHandler::WindowsFileHandler(const std::string path) : file_path_(path) {
	filename_ = get_file_name_from_full_path(file_path_);
	file_dir_ = file_path_;
	file_dir_ = file_dir_.substr(0, file_dir_.size() - filename_.size() - 1);
}

WindowsFileHandler::~WindowsFileHandler() { close_file(); }

bool WindowsFileHandler::open_file() {

	if ((file_handle_ = CreateFile(file_path_.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr)) != INVALID_HANDLE_VALUE)
		return true;

	return false;
}

void WindowsFileHandler::close_file() const {
	if (file_handle_ != INVALID_HANDLE_VALUE) {
		CloseHandle(file_handle_);
	}
}

DWORD WindowsFileHandler::get_file_size() const {
	if (file_handle_ != INVALID_HANDLE_VALUE)
		return GetFileSize(file_handle_, nullptr);
	return 0;
}

bool WindowsFileHandler::get_file_time(const LPFILETIME lp_creation_time, const LPFILETIME lp_last_access_time, const LPFILETIME lp_last_write_time) const {
	return GetFileTime(file_handle_, lp_creation_time, lp_last_access_time, lp_last_write_time);
}

std::string WindowsFileHandler::format_file_time(FILETIME filetime) {

	SYSTEMTIME st_utc, st_local;
	std::stringstream ss;

	// Convert the last-write time to local time.
	FileTimeToSystemTime(&filetime, &st_utc);
	SystemTimeToTzSpecificLocalTime(nullptr, &st_utc, &st_local);

	ss << std::setfill('0') << std::setw(2) << st_local.wMonth << "/" << std::setfill('0') << std::setw(2) << st_local.wDay << "/" << st_local.wYear << " " << std::setfill('0') << std::setw(2) << st_local.wHour << ":" << std::setfill('0') << std::setw(2) << st_local.wMinute << std::endl;

	return ss.str();
}

std::string WindowsFileHandler::get_filename() const {
	return this->filename_;
}

std::string WindowsFileHandler::get_file_path() const {
	return file_path_;
}

/*
 * Extract file name from the pull path. This algorithm extract the name starting from the end of the string representing the path
 */
std::string WindowsFileHandler::get_file_name_from_full_path(const std::string& file_path) {
	std::string file_name;

	const std::string::const_reverse_iterator it = std::find(file_path.rbegin(), file_path.rend(), '\\');

	if (it != file_path.rend()) {
		file_name.assign(file_path.rbegin(), it);
		std::reverse(file_name.begin(), file_name.end());
		return file_name;
	}
	
	return file_name;
}
