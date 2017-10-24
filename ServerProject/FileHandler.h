#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <fstream>
#include <future>
#include <string>
#include <boost\asio.hpp>

#include "FileOpenException.h"
#include "FileWriteException.h"

#define MAX_ATTEMPTS 5
#define TEMP_PATH std::string("C:\\users\\utentepc\\Appdata\\local\\temp")

/*
	this class handle the file object, the mothods are:
	open, close, write data.
*/
class FileHandler {
private:
	// path of the temp directory
	// TODO remember to write \\ instead of \ when you write the file path
	std::string file_dir;
	std::string file_path;

	std::ofstream file;
	std::string filename;

public:
	FileHandler(std::string filename, std::string path);
	FileHandler(char *filename, char *path);
	void openFile();
	void closeFile();
	void removeFile();
	void writeData(boost::asio::mutable_buffer& buffer);
	void copyFile(FileHandler dest_file);
};

#endif // !FILE_HANDLER_H
