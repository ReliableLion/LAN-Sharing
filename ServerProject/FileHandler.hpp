#pragma once

#include <fstream>
#include <string>
#include <sstream>	

#include "Exceptions.hpp"
#include "Constants.hpp"

enum file_open_type {WRITE, READ};

class FileHandler
{
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
	void openFile(int open_mode);
	bool closeFile();
	bool isOpen();
	bool removeFile();
	bool copyFile(FileHandler& dest);
	void writeData(const char *buffer, std::size_t size);
	void readFile(char *buffer, std::size_t size);
	std::string getFilename();
	std::string getFilePath();
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
	void readFile(char *buffer, std::size_t size);
};

class OutputFileHandler : public FileHandler
{
public:
	OutputFileHandler(std::string filename, std::string path);
	~OutputFileHandler();
	void openFile();
	void writeData(const char *buffer, std::size_t size);
};*/

class TemporaryFile : public FileHandler 
{
public:
	TemporaryFile(std::string filename, std::string path);
	~TemporaryFile();
};