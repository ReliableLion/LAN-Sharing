#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <fstream>
#include <future>
#include <string>
#include <boost\asio.hpp>

#include "FileOpenException.h"
#include "FileWriteException.h"

#define MAX_ATTEMPTS 5

/*
	this class handle the file object, the mothods are:
	open, close, write data.
*/

class FileHandler {
private:
	std::ofstream file;
	std::string filename;

public:
	FileHandler(std::string filename): filename(filename) {}
	FileHandler(char *filename) { this->filename = std::string(filename); }
	void openFile();
	void closeFile();
	void writeData(boost::asio::mutable_buffer& buffer);
};

#endif // !FILE_HANDLER_H
