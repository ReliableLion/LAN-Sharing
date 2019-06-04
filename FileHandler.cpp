#include <iostream>
#include <random>

#include "FileHandler.hpp"
#include "Exceptions.hpp"

FileHandler::FileHandler(const std::string filename, const std::string path) : file_dir_(path), filename_(filename), open_mode_(read) {
	std::stringstream ss;
	ss << file_dir_ << "\\" << filename_;
	file_path_ = ss.str();
}

FileHandler::FileHandler(const std::string filepath) : file_path_(filepath), open_mode_(read) {}

FileHandler::~FileHandler() { close_file(); }

bool FileHandler::is_open() const { return file_.is_open(); }

void FileHandler::open_file(const int open_mode) {

	if (file_.is_open())
		return;

	switch (open_mode) {
		case (write): {
			file_.open(file_path_, std::fstream::binary | std::fstream::out);                // open the file in binary mode
			this->open_mode_ = write;
		} break;
		case (read): {
			file_.open(file_path_, std::fstream::binary | std::fstream::in);
			this->open_mode_ = read;
		} break;
		default:
			throw FileOpenException();
	}

    if (!file_.good()) 
		throw FileOpenException();
}

void FileHandler::close_file() {
	if (file_.is_open())
		file_.close();

	// I must check if the file has been closed correctly
}

bool FileHandler::copy_file(FileHandler &dest) const {
	if (!(this->is_open() && dest.is_open()
		&& this->open_mode_ == read
		&& dest.open_mode_ == write))
		return false;

    dest.file_ << this->file_.rdbuf();
    return (this->file_.good() && dest.file_.good());
}

bool FileHandler::remove_file() {
    if (file_.is_open()) 
		this->close_file();

    if (remove(this->file_path_.c_str())) 
		return true;

    return false;
}

void FileHandler::write_data(std::shared_ptr<SocketBuffer> buffer) {

    if (!file_.is_open() || open_mode_ == read) 
		throw FileWriteException();

    int count = 0;

	// remember to check if the buffer contains data
	if (buffer->get_size() == 0 || buffer->get_remaining_bytes() == 0)
		throw FileWriteException();

    do {
        file_.write(buffer->read_from_buffer(), buffer->get_remaining_bytes());
        count++; 
#ifdef FILE_HANDLER_OUTPUT
		if (!file_.good())
			std::cout << "impossible to write into the file" << std::endl;
#endif
    } while (!file_.good() && count < MAX_ATTEMPTS_);

    if (count == MAX_ATTEMPTS_) 
		throw FileWriteException();

	// update the read pointer into the Buffer 
	buffer->read_all_bytes();
}

void FileHandler::read_data(std::shared_ptr<SocketBuffer> buffer) {}

/*
 * Check if it's possible to write to the current location
 */
bool FileHandler::check_write_permission() {
	std::ofstream temp_w(file_path_);
	if (temp_w.good()) {
		temp_w.close();
		std::remove(file_path_.c_str());
		return true;
	} else {
#ifdef FILE_HANDLER_OUTPUT
	std::cout << "impossible to write file " << file_path_.c_str() << std::endl;
#endif
		return false;
	}
}

std::string FileHandler::get_filename() const { return filename_; }

std::string FileHandler::get_filepath() const { return file_path_; }

bool FileHandler::rename_file(std::string new_name) {

	if (file_.is_open()) {
#ifdef FILE_HANDLER_OUTPUT
		std::cout << "impossible to rename the file because is already open" << std::endl;
#endif
		return false;
	}

	int result = rename(filename_.c_str(), new_name.c_str());

	if (result == 0) {
#ifdef FILE_HANDLER_OUTPUT
		std::cout << "the file is correclty renamed" << std::endl;
#endif
		filename_ = new_name;
		std::stringstream ss;
		ss << file_dir_ << "\\" << filename_;
		file_path_ = ss.str();
		return false;
	}

#ifdef FILE_HANDLER_OUTPUT
	std::cout << "impossible to rename the file" << std::endl;
#endif
	return true;
}

TemporaryFile::TemporaryFile() : FileHandler(std::string(""), TEMP_PATH) {
	// generate a random file name 
	std::string rand_filename = generate_random_filename(20);
	filename_ = rand_filename;

	std::stringstream ss;
	ss << file_dir_ << "\\" << filename_;
	file_path_ = ss.str();
}

TemporaryFile::~TemporaryFile() {
	close_file();
    remove_file();
}

// it's better to move this function in another file
std::string TemporaryFile::generate_random_filename(int len) {
	constexpr char chset[] =
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	auto r_char = [=, r_gen{ std::mt19937{ std::random_device{}() } }]() mutable {
		return chset[std::uniform_int_distribution<>{0, sizeof chset - 1}(r_gen)];
	};

	std::stringstream ss;

	for (int i = 0; i < 20; i++) {
		ss << r_char();
	}

	ss << ".tmp";
	return ss.str();
}