#include "FileHandler.hpp"

FileHandler::FileHandler(std::string filename, std::string path) : filename(filename), file_dir(path) {}

FileHandler::~FileHandler() { closeFile(); }

/*FileHandler::FileHandler(co t FileHandler& filehandler) {
//TODO remember to complete the copy constructor
// creation of a copy
}
FileHandler::FileHandler(const FileHandler&& filehandler) {
// TODO remember to complete the move constructor
}*/

bool FileHandler::closeFile() {

	if (file.is_open()) {
		file.close();
		return true;
	}
	else {
		return false;
	}
}

/**
* \brief
* \param dest_file
* \return
*/
bool FileHandler::copyFile(FileHandler& dest_file) {
	std::ifstream src;
	std::ofstream dest;

	src.open(this->filename, std::fstream::binary);
	dest.open(dest_file.getFilename(), std::fstream::binary);

	dest << src.rdbuf();
	return (src.good() && dest.good());
}


void FileHandler::removeFile() {
	remove(this->filename.c_str());
}

std::string FileHandler::getFilename() { return this->filename; }

InputFileHandler::InputFileHandler(std::string filename, std::string path) : FileHandler(filename, path) {}

InputFileHandler::~InputFileHandler() {
	closeFile();
}

bool InputFileHandler::openFile() {
	if (filename.empty() || file_dir.empty()) {				//check if the filename or the file path are empty
		throw file_open_exception();
	}

	file_path = file_dir.append("\\").append(filename);							// pathname definition

	file.open(file_path, std::fstream::binary, std::fstream::in);				// open the file in binary mode 
	if (!file.good()) {
		throw file_open_exception();
	}
	return true;
}

void InputFileHandler::readFile(char *buffer, std::size_t size) {

}

OutputFileHandler::OutputFileHandler(std::string filename, std::string path) : FileHandler(filename, path) {}

OutputFileHandler::~OutputFileHandler() {
	closeFile();
}

bool OutputFileHandler::openFile() {
	if (filename.empty() || file_dir.empty()) {				//check if the filename or the file path are empty
		throw file_open_exception();
	}

	file_path = file_dir.append("\\").append(filename);							// pathname definition

	file.open(file_path, std::fstream::binary, std::fstream::out);				// open the file in binary mode 
	if (!file.good()) {
		throw file_open_exception();
	}
	return true;
}

void OutputFileHandler::writeData(const char *buffer, std::size_t size) {
	std::size_t n_byte = size;
	int count = 0;

	if (buffer == nullptr) {
		throw file_write_exception();
	}

	if (!file.is_open()) {						// if file is not open, try to open it, otherwisw throw a new FileWrite Eception
		if (!openFile()) {
			throw file_write_exception();
		}
	}

	do {
		file.write(buffer, n_byte);
		count++;
	} while (!file.good() || count < maxAttempts);

	if (count == maxAttempts) {
		throw file_write_exception();
	}


}

TransmitFileHandler::TransmitFileHandler(std::string filename, std::string path) : FileHandler(filename, path) {}

TransmitFileHandler::~TransmitFileHandler() {
	CloseHandle(handle_file_);
}

bool TransmitFileHandler::openFile() {

	if (filename.empty() || file_dir.empty()) {				//check if the filename or the file path are empty
		throw file_open_exception();
	}

	file_path = file_dir.append("\\").append(filename);							// pathname definition

	handle_file_ = CreateFile(file_path.c_str(),               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
	    nullptr,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_FLAG_OVERLAPPED & FILE_FLAG_SEQUENTIAL_SCAN, // normal file
		nullptr);                 // no attr. template

	if (handle_file_ == INVALID_HANDLE_VALUE) {
		throw file_open_exception();
	}

	return true;
}

bool TransmitFileHandler::transmit_file(const SOCKET socket) const {

	if (!TransmitFile(socket, handle_file_, 0, CHUNK, nullptr, nullptr, TF_USE_SYSTEM_THREAD))
		return true;
	throw transmit_file_exception(WSAGetLastError());
}
