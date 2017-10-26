#include "FileHandler.h"

FileHandler::FileHandler(std::string filename, std::string path) : filename(filename), file_dir(path) {
	file_path = file_dir.append("\\").append(this->filename);
}

FileHandler::FileHandler(char *filename, char *path) {
	this->file_dir = std::string(path);
	this->filename = std::string(filename);
	file_path = file_dir.append("\\").append(this->filename);
}

FileHandler::~FileHandler() {
	closeFile();
}

// open the file nad check if it is ready to be used
// if t isn't possible to open the file lauch a new open file exception
void FileHandler::openFile() {
	
	// open the file in binary mode 
	file.open(file_path, std::fstream::binary);
	
	if (!file.is_open()) {
		//lauch a new FileOpenException to v
		//throw FileOpenException();
	}

}

// close the file stream and lauch an exception if there are some problem 
void FileHandler::closeFile() {

	if (file.is_open()) {
		file.close();
	}
	else {
		// throw a new exception if the file is not open
		throw FileOpenException();
	}

}

// this method receive as parameter the buffer and write it on the file
void FileHandler::writeData(std::shared_ptr<buffer_type> buffer, size_t size) {
	std::size_t n_byte = size;
	int count = 0;
	
	//check if the buffer is empty or not
	if (buffer == nullptr) {
		// throw a new exception if the buffer is null
		throw FileWriteException();
	}

	file.write(*buffer, n_byte);
	while (!file.good() || count < MAX_ATTEMPTS) {
		file.write(write_data, n_byte);
		count++;
	}

	if (count == MAX_ATTEMPTS) {
		// error: is not possible to write the data into the file throw a new exception 
		throw FileWriteException();
	}

}

bool FileHandler::copyFile(FileHandler& dest_file) {
	std::ifstream src;
	std::ofstream dest;

	src.open(this->filename, std::fstream::binary);
	dest.open(dest_file.getFilename, std::fstream::binary);

	dest << src.rdbuf();
	return (src.good() && dest.good());
}

// to be used after the file closing
void FileHandler::removeFile() {
	remove(this->filename.c_str());
}

std::string FileHandler::getFilename() { return this->filename; }