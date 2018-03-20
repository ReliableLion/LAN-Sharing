#include "stdafx.h"

#include "FileManagerWin32.h"

FileManager::FileManager(std::string filename)
{
	if(!filename.empty())
	{
		// get the file name and trasform it into a 
		this->filename = filename;
		this->win_filename = (TCHAR*)filename.c_str();
	}
}

FileManager::~FileManager()
{
	closeFile();
}



bool FileManager::openFile(){
	hFile = CreateFile(win_filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "Impossible to open the file: " << std::endl;
		this->isOpen = false;
		return false;
	} else
	{
		this->isOpen = true;
		return true;
	}
}

void FileManager::closeFile()
{
	if(isOpen)
	{
		CloseHandle(hFile);
	} 
}

bool FileManager::getFileTimeStamp()
{
	return (bool) GetFileTime(hFile, creationTime, lastAccess, lastModified);
}

bool FileManager::getFileSize()
{
	return (bool) GetFileSizeEx(hFile, &fileSize);
}

