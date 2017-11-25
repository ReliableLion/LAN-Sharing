#pragma once
#include "stdafx.h"
#include "FileOpenException.h"
#include "FileWriteException.h"
#include <fstream>
#include <string>

class FileHandler {
protected:
	std::string file_dir;								// TODO remember to write \\ instead of \ when you write the file path
	std::string file_path;
	std::string filename;
	std::fstream file;									// generic file stream for write and read operation on files
	const int maxAttempts = MAX_FILE_ATTEMPTS;
public:
	FileHandler() {};
	FileHandler(std::string filename, std::string path);
	virtual ~FileHandler();
	virtual bool openFile() =0;
	bool closeFile();
	void removeFile();
	bool copyFile(FileHandler& dest_file);
	std::string getFilename();
	/*FileHandler(char *filename, char *path);
	FileHandler(const FileHandler&& filehandler);
	FileHandler(const FileHandler& filehandler);*/
};

class InputFileHandler : public FileHandler {
public:
	InputFileHandler(std::string filename, std::string path);
	~InputFileHandler();
	bool openFile();
	void readFile(char *buffer, std::size_t size);
};

class OutputFileHandler : public FileHandler {
public:
	OutputFileHandler(std::string filename, std::string path);
	~OutputFileHandler();
	bool openFile();
	void writeData(const char *buffer, std::size_t size);
};