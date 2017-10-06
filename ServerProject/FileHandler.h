#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <fstream>
#include <future>
#include <string>
#include <boost\asio.hpp>

#include "FileOpenExceptio.h"

#define MAX_ATTEMPTS 5

class FileHandler {
private:
	std::ofstream file;
	std::string filename;

public:
	FileHandler(std::string filename) : filename(filename) {}
	FileHandler(char *filename) { this->filename = std::string(filename); }
	void openFile();
	void closeFile();
	void writeData(boost::asio::mutable_buffer& buffer);

};

#endif // !FILE_HANDLER_H
