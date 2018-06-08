#pragma once

#include <fstream>
#include <string>
#include <memory>

#include "Constants.hpp"
#include "SocketBuffer.hpp"

enum file_open_type {
    write, read
};

class FileHandler {
private:
    FileHandler(FileHandler &source): open_mode_() {} ;

    //FileHandler &operator=(FileHandler &source) {};

protected:
    std::string file_dir_;                                // TODO remember to write \\ instead of \ when you write the file path
    std::string filename_;
    std::string file_path_;
    std::fstream file_;                                    // generic file stream for write and read operation on files
    const int MAX_ATTEMPTS_ = MAX_FILE_ATTEMPTS;
    file_open_type open_mode_;

public:
    FileHandler(std::string filename, std::string path);

    ~FileHandler();

    void open_file(int open_mode);

    bool close_file();

    bool is_open() const;

    bool remove_file();

    bool copy_file(FileHandler &dest) const;

    void write_data(std::shared_ptr<SocketBuffer> buffer);

    //  TODO not implemented
	static void read_data(std::shared_ptr<SocketBuffer> buffer);

    std::string get_filename() const;

    std::string get_filepath();
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
void read_file(char *buffer, std::size_t size);
};

class OutputFileHandler : public FileHandler
{
public:
OutputFileHandler(std::string filename, std::string path);
~OutputFileHandler();
void openFile();
void write_data(const char *buffer, std::size_t size);
};*/

class TemporaryFile : public FileHandler {
public:
	// TODO sostituire il path dopo il debug
	TemporaryFile::TemporaryFile(std::string filename, std::string path) : FileHandler(filename, TEMP_PATH) {};

    ~TemporaryFile();
};