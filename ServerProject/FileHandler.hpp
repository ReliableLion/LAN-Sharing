#pragma once
#include "stdafx.h"
#include "FileOpenException.h"
#include "FileWriteException.h"
#include <fstream>
#include <string>

#define MAX_ATTEMPTS 5
#define TEMP_PATH std::string("C:\\users\\utentepc\\Appdata\\local\\temp")

class FileHandler {
protected:
	// TODO remember to write \\ instead of \ when you write the file path
	std::string file_dir;
	std::string file_path;
	std::string filename;
	std::fstream file;			// file stream
public:
	FileHandler() {};
	FileHandler(std::string filename, std::string path);
	~FileHandler();
	virtual bool openFile() =0;
	bool closeFile();
	void removeFile();
	void writeData(const char *buffer, size_t size);
	bool copyFile(FileHandler& dest_file);
	std::string getFilename();
	/*FileHandler(char *filename, char *path);
	FileHandler(const FileHandler&& filehandler);
	FileHandler(const FileHandler& filehandler);*/
};

class InputFileHandler : public FileHandler {
public:
	InputFileHandler(std::string filename, std::string path);
	bool openFile();
	void readFile();
};

class OutputFileHandler : public FileHandler {
public:
	OutputFileHandler(std::string filename, std::string path);
	bool openFile();
	void writeData(const char *buffer, size_t size);
};