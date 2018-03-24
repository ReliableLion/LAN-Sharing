#include "stdafx.h"

#include "FileHandler.hpp"

FileHandler::FileHandler(std::string filename, std::string path) : filename(filename), file_dir(path), file_path("") {}

FileHandler::~FileHandler() { closeFile(); }

bool FileHandler::isOpen() { return file.is_open(); }

void FileHandler::openFile(int open_mode)
{
	if (filename.empty() || file_dir.empty())	throw FileOpenException();

	std::stringstream ss;
	ss << file_dir << "\\" << filename;
	file_path = ss.str();

	if (open_mode == WRITE)
	{
		file.open(file_path, std::fstream::binary | std::fstream::out);				// open the file in binary mode 
		this->open_mode = WRITE;
	}
	else
	{
		file.open(file_path, std::fstream::binary | std::fstream::in);
		this->open_mode = READ;
	}

	if (!file.good())	throw FileOpenException();
}

bool FileHandler::closeFile()
{
	if (file.is_open()) 
	{
		file.close();
		return true;
	}
	
	return false;
}

/**
 * \brief 
 * \param dest_file 
 * \return 
 */
bool FileHandler::copyFile(FileHandler& dest)
{
	// return true if the 2 file are the same
	if(&dest == this)
		return true;
	

	if(!this->file.is_open())
	{
		this->openFile(READ);
		this->open_mode = READ;
	}

	if(this->file.is_open() && this->open_mode == WRITE)
	{
		this->closeFile();
		this->openFile(READ);
		this->open_mode = READ;
	}

	if (!dest.file.is_open())
	{
		this->openFile(READ);
		this->open_mode = READ;
	}

	if (dest.file.is_open() && dest.open_mode == READ)
	{
		this->closeFile();
		this->openFile(READ);
		this->open_mode = READ;
	}

	// TODO.: search better method for file transfert
	dest.file << this->file.rdbuf();
	return (this->file.good() && dest.file.good());
}
						
bool FileHandler::removeFile()
{			
	int res = remove(this->file_path.c_str());
	if(res == 0)
	{
		return true;
	} else
	{
		return false;
	}
}

void FileHandler::writeData(const char *buffer, std::size_t size)
{
	std::size_t n_byte = size;
	int count = 0;

	if (buffer == nullptr || size < 0)	throw FileWriteException();

	if (!file.is_open())
	{
		openFile(WRITE);					// if file is not open, try to open it, otherwisw throw a new FileWrite Exception	
		open_mode = WRITE;
	}

	if(file.is_open() && open_mode == READ)
	{
		closeFile();
		openFile(WRITE);
		open_mode = WRITE;
	}

	do
	{
		file.write(buffer, n_byte);
		count++;
	} while (!file.good() && count < maxAttempts);

	if (count == maxAttempts)	throw FileWriteException();
}

void FileHandler::readFile(char *buffer, std::size_t size)
{

}

std::string FileHandler::getFilename() { return this->filename; }

std::string FileHandler::getFilePath()
{
	if(file_path.empty())
	{
		std::stringstream ss;
		ss << file_dir << "\\" << filename;
		return (file_path = ss.str());
	} 

	return file_path;
}




/*OutputFileHandler::OutputFileHandler(std::string filename, std::string path) : FileHandler(filename, path) {}

OutputFileHandler::~OutputFileHandler()
{
	closeFile();
}

void OutputFileHandler::openFile()
{
	if (filename.empty() || file_dir.empty())	throw FileOpenException();
	
	std::stringstream ss;														// pathname definition
	ss << file_dir << "\\" << filename;
	file_path = ss.str();

	file.open(file_path, std::fstream::binary, std::fstream::out);				// open the file in binary mode 

	if (!file.good())	throw FileOpenException();
}

/**
 * \brief this method is in change to write the buffer into the file, raise FileWriteException if there are some error 
 * \param buffer: the chunk of data to be written into the file
 * \param size: the size of the data to be written
 */

TemporaryFile::TemporaryFile(std::string filename, std::string path) : FileHandler(filename, path) {}

TemporaryFile::~TemporaryFile()
{
	// if the destructor is invoked for an instance of the temporary file 
	// this will close the file and remove it from 
	if (file.is_open()) file.close();
		removeFile();
}


