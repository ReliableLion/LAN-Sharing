#pragma once

#include <fstream>
#include <string>
#include <sstream>

#include "Exceptions.hpp"
#include "Constants.hpp"
#include "stdafx.h"
#include <memory>
#include "SocketBuffer.hpp"

enum file_open_type { WRITE, READ };

class FileHandler {
	FileHandler(FileHandler& source) {};
	FileHandler& operator= (FileHandler& source) {};
protected:
	std::string file_dir_;								// TODO remember to write \\ instead of \ when you write the file path
	std::string filename_;
	std::string file_path_;
	std::fstream file_;									// generic file stream for write and read operation on files
	const int max_attempts_ = MAX_FILE_ATTEMPTS;
	file_open_type open_mode_;

public:
	FileHandler(std::string filename, std::string path);
	~FileHandler();
	void open_file(int open_mode);
	bool close_file();
	bool is_open();
	bool remove_file();
	bool copy_file(FileHandler& dest);
	void write_data(std::shared_ptr<SocketBuffer> buffer);
	void read_file(std::shared_ptr<SocketBuffer> buffer);
	std::string get_filename();
	std::string get_FilePath();
	/*FileHandler(char *filename, char *path);
	FileHandler(const FileHandler&& filehandler);
	FileHandler(const FileHandler& filehandler);*/
};

/*class InputFileHandler : public FileHandler
{
public:
InputFileHandler(std::string filename, std::string path);
~InputFileHandler();
void openFile();
void read_file(char *buffer, std::size_t size);
};

class OutputFileHandler : public FileHandler
{
public:
OutputFileHandler(std::string filename, std::string path);
~OutputFileHandler();
void openFile();
void write_data(const char *buffer, std::size_t size);
};*/

class TemporaryFile : public FileHandler {
public:
	TemporaryFile(std::string filename, std::string path);
	~TemporaryFile();
};