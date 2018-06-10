#include "FileHandler.hpp"
#include "Exceptions.hpp"
#include <iostream>

FileHandler::FileHandler(const std::string filename, const std::string path) : file_dir_(path), filename_(filename), file_path_(""), open_mode_(read) {}

FileHandler::~FileHandler() { close_file(); }

bool FileHandler::is_open() const { return file_.is_open(); }

void FileHandler::open_file(const int open_mode) {
    if (filename_.empty() || file_dir_.empty()) throw FileOpenException();

    std::stringstream ss;
    ss << file_dir_ << "\\" << filename_;
    file_path_ = ss.str();

    if (open_mode == write) {
        file_.open(file_path_, std::fstream::binary | std::fstream::out);                // open the file in binary mode
        this->open_mode_ = write;
    } else {
        file_.open(file_path_, std::fstream::binary | std::fstream::in);
        this->open_mode_ = read;
    }

    if (!file_.good()) throw FileOpenException();
}

bool FileHandler::close_file() {
    if (file_.is_open()) {
        file_.close();
        return true;
    }

    return false;
}

bool FileHandler::copy_file(FileHandler &dest) const {
    if (!(this->is_open() && dest.is_open()
          && this->open_mode_ == read && dest.open_mode_ == write))
        throw FileWriteException();

    dest.file_ << this->file_.rdbuf();
    return (this->file_.good() && dest.file_.good());
}

bool FileHandler::remove_file() {
    if (file_.is_open()) this->close_file();

    if (remove(this->file_path_.c_str())) return true;

    return false;
}

void FileHandler::write_data(std::shared_ptr<SocketBuffer> buffer) {

    // la logica è questa: il buffer che passo come parametro decide le regole,
    // infatti è quest'ulimo che decide la dimensione del buffer locale da utilizzare e
    // la quantità di byte da leggere dal file

    if (!file_.is_open() || open_mode_ == read) throw FileWriteException();

    int count = 0;

    do {
        file_.write(buffer->get_remaining_data(), buffer->get_size());
        count++;

		if (!file_.good()) 
			std::cout << "impossible to write into the file" << std::endl;

    } while (!file_.good() && count < MAX_ATTEMPTS_);

    if (count == MAX_ATTEMPTS_) throw FileWriteException();
}

void FileHandler::read_data(std::shared_ptr<SocketBuffer> buffer) {}

std::string FileHandler::get_filename() const { return this->filename_; }

std::string FileHandler::get_filepath() {

    if (file_path_.empty()) {
        std::stringstream ss;
        ss << file_dir_ << "\\" << filename_;
        return (file_path_ = ss.str());
    }

    return file_path_;
}

bool FileHandler::rename_file(std::string new_name) {
	int result = rename(filename_.c_str(), new_name.c_str());

	if (result == 0) {
		std::cout << "the file is correclty renamed..." << std::endl;
		return false;
	}
		
	std::cout << "the file is not renamed" << std::endl;
	return true;
}


TemporaryFile::~TemporaryFile() {

    // if the destructor is invoked for an instance of the temporary file
    // this will close the file and remove it from
    if (file_.is_open())
		file_.close();

    //remove_file();
}