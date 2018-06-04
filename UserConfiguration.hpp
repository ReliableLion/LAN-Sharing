#pragma once
#include <fstream>
#include <string>

/*
this class is the main interface between the UI and the system
*/
class UserConfiguration {
private:
	bool is_public_;
	bool automatic_accept_;
	bool use_default_path_;

	std::string default_path_;
	std::string file_path_;
	std::string file_name_;

	// the use of the configuration file is limited to a test of the system
	std::ifstream write_config_file_;
	std::ofstream read_configu_file_;

	//TODO: create  a link to communicate with the UI of the application
public:
	UserConfiguration();
	void set_automatic_accept(bool value);
	static void set_default_path(bool use_default_path, std::string pathname);
	std::string get_default_path() const;
	std::string get_file_path() const;
	bool get_automatic_accept() const;
	bool get_use_default_path() const;
	bool get_is_public() const;
};
