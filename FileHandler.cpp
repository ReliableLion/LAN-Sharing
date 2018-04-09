#include "FileHandler.hpp"

FileHandler::FileHandler(std::string filename, std::string path) : filename_(filename), file_dir_(path), file_path_("") {}

FileHandler::~FileHandler() { closeFile(); }

HANDLE FileHandler::get_file_handle() {

	std::stringstream ss;
	ss << file_dir_ << "\\" << filename_;
	file_path_ = ss.str();

	return CreateFile(file_path_.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr);
}

bool FileHandler::isOpen() { return file_.is_open(); }

void FileHandler::openFile(int open_mode) {
	if (filename_.empty() || file_dir_.empty())	throw FileOpenException();

	std::stringstream ss;
	ss << file_dir_ << "\\" << filename_;
	file_path_ = ss.str();

	if (open_mode == WRITE) {
		file_.open(file_path_, std::fstream::binary | std::fstream::out);				// open the file in binary mode 
		this->open_mode_ = WRITE;
	}
	else {
		file_.open(file_path_, std::fstream::binary | std::fstream::in);
		this->open_mode_ = READ;
	}

	if (!file_.good())	throw FileOpenException();
}

bool FileHandler::closeFile() {
	if (file_.is_open()) {
		file_.close();
		return true;
	}

	return false;
}

/**
* \brief
* \param dest_file
* \return
*/
bool FileHandler::copyFile(FileHandler& dest) {
	// return true if the 2 file are the same
	if (&dest == this)
		return true;


	if (!this->file_.is_open()) {
		this->openFile(READ);
		this->open_mode_ = READ;
	}

	if (this->file_.is_open() && this->open_mode_ == WRITE) {
		this->closeFile();
		this->openFile(READ);
		this->open_mode_ = READ;
	}

	if (!dest.file_.is_open()) {
		this->openFile(READ);
		this->open_mode_ = READ;
	}

	if (dest.file_.is_open() && dest.open_mode_ == READ) {
		this->closeFile();
		this->openFile(READ);
		this->open_mode_ = READ;
	}

	// TODO.: search better method for file transfert
	dest.file_ << this->file_.rdbuf();
	return (this->file_.good() && dest.file_.good());
}

bool FileHandler::removeFile() {
	int res = remove(this->file_path_.c_str());
	if (res == 0) {
		return true;
	}
	else {
		return false;
	}
}

void FileHandler::writeData(const char *buffer, std::size_t size) {
	std::size_t n_byte = size;
	int count = 0;

	if (buffer == nullptr || size < 0)	throw FileWriteException();

	if (!file_.is_open()) {
		openFile(WRITE);					// if file is not open, try to open it, otherwisw throw a new FileWrite Exception	
		open_mode_ = WRITE;
	}

	if (file_.is_open() && open_mode_ == READ) {
		closeFile();
		openFile(WRITE);
		open_mode_ = WRITE;
	}

	do {
		file_.write(buffer, n_byte);
		count++;
	} while (!file_.good() && count < max_attempts_);

	if (count == max_attempts_)	throw FileWriteException();
}

void FileHandler::readFile(char *buffer, std::size_t size) {

}

std::string FileHandler::getFilename() { return this->filename_; }

std::string FileHandler::getFilePath() {
	if (file_path_.empty()) {
		std::stringstream ss;
		ss << file_dir_ << "\\" << filename_;
		return (file_path_ = ss.str());
	}

	return file_path_;
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

TemporaryFile::~TemporaryFile() {
	// if the destructor is invoked for an instance of the temporary file 
	// this will close the file and remove it from 
	if (file_.is_open()) file_.close();
	removeFile();
}


