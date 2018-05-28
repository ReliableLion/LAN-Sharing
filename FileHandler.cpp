#include "stdafx.h"

#include "FileHandler.hpp"

FileHandler::FileHandler(std::string filename, std::string path) : filename_(filename), file_dir_(path), file_path_(""), open_mode_(READ) {}

FileHandler::~FileHandler() { close_file(); }

bool FileHandler::is_open() { return file_.is_open(); }

void FileHandler::open_file(int open_mode)
{
	if (filename_.empty() || file_dir_.empty())	throw FileOpenException();

	std::stringstream ss;
	ss << file_dir_ << "\\" << filename_;
	file_path_ = ss.str();

	if (open_mode == WRITE)
	{
		file_.open(file_path_, std::fstream::binary | std::fstream::out);				// open the file in binary mode 
		this->open_mode_ = WRITE;
	}
	else
	{
		file_.open(file_path_, std::fstream::binary | std::fstream::in);
		this->open_mode_ = READ;
	}

	if (!file_.good())	throw FileOpenException();
}

bool FileHandler::close_file()
{
	if (file_.is_open())
	{
		file_.close();
		return true;
	}

	return false;
}

/**
* \brief
* \param dest_file: destination file of the copy
* \return
*/
bool FileHandler::copy_file(FileHandler& dest)
{
	if (!(this->is_open() && dest.is_open()
		&& this->open_mode_ == READ && dest.open_mode_ == WRITE))
		throw FileWriteException();

	dest.file_ << this->file_.rdbuf();
	return (this->file_.good() && dest.file_.good());
}

bool FileHandler::remove_file()
{
	if (file_.is_open()) this->close_file();

	if (remove(this->file_path_.c_str())) return true;

	return false;
}

void FileHandler::write_data(std::shared_ptr<SocketBuffer> buffer)
{

	// la logica è questa: il buffer che passo come parametro decide le regole,
	// infatti è quest'ulimo che decide la dimensione del buffer locale da utilizzare e
	// la quantità di byte da leggere dal file

	if (!file_.is_open() || open_mode_ == READ)	throw FileWriteException();

	int count = 0;

	do {
		file_.write(buffer->get_buffer(), buffer->get_size());
		count++;
	} while (!file_.good() && count < max_attempts_);

	if (count == max_attempts_) throw FileWriteException();
}

void FileHandler::read_data(std::shared_ptr<SocketBuffer> buffer)
{

}

std::string FileHandler::get_filename() { return this->filename_; }

std::string FileHandler::get_filepath()
{

	if (file_path_.empty())
	{
		std::stringstream ss;
		ss << file_dir_ << "\\" << filename_;
		return (file_path_ = ss.str());
	}

	return file_path_;
}


/*OutputFileHandler::OutputFileHandler(std::string filename, std::string path) : FileHandler(filename, path) {}

OutputFileHandler::~OutputFileHandler()
{
close_file();
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
	if (file_.is_open()) file_.close();

	remove_file();
}