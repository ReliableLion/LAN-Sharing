#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <fstream>
#include <future>
#include <string>
#include <boost\asio.hpp>

#include "FileOpenException.h"
#include "FileWriteException.h"
#include "TCPconnection.h"
#include "Buffer.h"

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
	// the file dir depend on the user configuration
	std::string file_dir;
	std::string file_path;
	std::string filename;
	// file object
	std::ofstream file;
public:
	FileHandler() {};
	FileHandler(std::string filename, std::string path);
	FileHandler(char *filename, char *path);
	FileHandler(const FileHandler&& filehandler);
	FileHandler(const FileHandler& filehandler);
	~FileHandler();
	FileHandler& operator=(const FileHandler& filehandler);
	bool openFile();
	void closeFile();
	void removeFile();
	void writeData(std::shared_ptr<buffer_type> buffer, size_t size);
	bool copyFile(FileHandler& dest_file);
	std::string getFilename();
};

#endif // !FILE_HANDLER_H
