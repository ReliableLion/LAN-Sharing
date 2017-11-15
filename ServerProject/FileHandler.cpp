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

bool InputFileHandler::openFile() {
	if (filename.empty() || file_dir.empty()) {				//check if the filename or the file path are empty
		throw FileOpenException();
	}

	file_path = file_dir.append("\\").append(filename);							// pathname definition

	file.open(file_path, std::fstream::binary, std::fstream::in);				// open the file in binary mode 
	if (!file.is_open()) {
		throw FileOpenException();
	}
	return true;
}

OutputFileHandler::OutputFileHandler(std::string filename, std::string path) : FileHandler(filename, path) {}

bool OutputFileHandler::openFile() {
	if (filename.empty() || file_dir.empty()) {				//check if the filename or the file path are empty
		throw FileOpenException();
	}

	file_path = file_dir.append("\\").append(filename);							// pathname definition

	file.open(file_path, std::fstream::binary, std::fstream::out);				// open the file in binary mode 
	if (!file.is_open()) {
		throw FileOpenException();
	}
	return true;
}

void OutputFileHandler::writeData(const char *buffer, size_t size) {
	std::size_t n_byte = size;
	int count = 0;

	if (buffer == nullptr) {
		throw FileWriteException();
	}

	file.write(buffer, n_byte);
	while (!file.good() || count < MAX_ATTEMPTS) {
		file.write(buffer, n_byte);
		count++;
	}

	if (count == MAX_ATTEMPTS) {
		throw FileWriteException();
	}
}