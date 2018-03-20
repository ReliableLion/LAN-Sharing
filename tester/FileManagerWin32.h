#pragma once

#include "stdafx.h"

#include <iostream>
#include <string>

#include "Windows.h"

class FileManager
{
	std::string filename;
	const  TCHAR *win_filename;
	HANDLE hFile;

	bool isOpen;

	LPFILETIME lastModified;
	LPFILETIME lastAccess;
	LPFILETIME creationTime;

	LARGE_INTEGER fileSize = { 0 };

public:
	FileManager(std::string filename);
	~FileManager();

	bool openFile();
	void closeFile();
	bool getFileTimeStamp();
	bool getFileSize();
	__int64 getSize() { return fileSize.QuadPart; }
	LPFILETIME getLastModified() { return lastModified; };
	LPFILETIME getLastAccess() { return lastAccess; };
	LPFILETIME getcreationTime() { return creationTime; };
};
