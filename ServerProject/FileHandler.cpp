#include "FileHandler.h"

FileHandler::FileHandler(std::string filename, std::string path) : filename(filename), file_dir(path) {}

FileHandler::FileHandler(char *filename, char *path) {
	this->file_dir = std::string(path);
	this->filename = std::string(filename);
}

FileHandler::~FileHandler() {
	closeFile();
}

FileHandler::FileHandler(const FileHandler& filehandler) {
	//TODO remember to complete the copy constructor
	// creation of a copy 

}

FileHandler::FileHandler(const FileHandler&& filehandler) {
	// TODO remember to complete the move constructor
}

FileHandler& FileHandler::operator=(const FileHandler& filehandler) {
	if (&filehandler != this) {
		// TODO remember to complete the copy operator
		this->file_path = filehandler.file_path;
		this->file_dir = filehandler.file_dir;
		this->filename = filehandler.filename;
		return *this;
	}
}

// open the file nad check if it is ready to be used
// if t isn't possible to open the file lauch a new open file exception
bool FileHandler::openFile() {
	
	//check if the filename or the file path are empty
	if (filename.empty() || file_dir.empty()) {
		throw FileOpenException();
	}

	// create the file path 
	file_path = file_dir.append("\\").append(filename);

	// open the file in binary mode 
	file.open(file_path, std::fstream::binary);
	if (!file.is_open()) {
		//lauch a new FileOpenException
		throw FileOpenException();
	}
	return true;
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
	
	// with data i mean the byte read from the socket 
	// size is the lenght of the data received from the socket
	std::size_t n_byte = size;
	const char *data;
	int count = 0;
	
	if (buffer == nullptr) {
		throw FileWriteException();
	}

	data = ArraytoString(*buffer);

	file.write(data, n_byte);
	while (!file.good() || count < MAX_ATTEMPTS) {
		file.write(data, n_byte);
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
	dest.open(dest_file.getFilename(), std::fstream::binary);

	dest << src.rdbuf();
	return (src.good() && dest.good());
}

// to be used after the file closing
void FileHandler::removeFile() {
	remove(this->filename.c_str());
}

std::string FileHandler::getFilename() { return this->filename; }