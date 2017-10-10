#include "FileHandler.h"

// open the file nad check if it is ready to be used
// if t isn't possible to open the file lauch a new open file exception
void FileHandler::openFile() {
	
	// open the file in binary mode 
	file.open(filename, std::fstream::binary);
	
	if (!file.is_open()) {
		//lauch a new FileOpenException to v
		throw FileOpenException();
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
void FileHandler::writeData(boost::asio::mutable_buffer& buffer) {
	std::size_t n_byte;
	char *write_data;
	int count = 0;
	
	n_byte = boost::asio::buffer_size(buffer);
	write_data = boost::asio::buffer_cast<char *> (buffer);
	
	//check if the buffer is empty or not
	if (write_data = NULL) {
		// throw a new exception if the buffer is null
		throw FileWriteException();
	}

	file.write(write_data, n_byte);
	while (!file.good() || count < MAX_ATTEMPTS) {
		file.write(write_data, n_byte);
		count++;
	}

	if (count == MAX_ATTEMPTS) {
		// error: is not possible to write the data into the file throw a new exception 
		throw FileWriteException();
	}

}