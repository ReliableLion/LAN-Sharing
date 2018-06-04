#include "UserConfiguration.hpp"

UserConfiguration::UserConfiguration(): is_public_(false), automatic_accept_(false), use_default_path_(false) {
}

void UserConfiguration::set_automatic_accept(const bool value) {

	//TODO: in this section i will add the automatic accept taken from the interface and place it into a configuration file and in the UserConfiguration instance 
	this->automatic_accept_ = value;
}

void UserConfiguration::set_default_path(bool use_default_path, std::string pathname) {
	//TODO:	add the default path option in the UserConfiguration instance and add it into the configuration
}

std::string UserConfiguration::get_file_path() const { return this->file_path_; }

std::string UserConfiguration::get_default_path() const { return this->default_path_; }

bool UserConfiguration::get_automatic_accept() const { return this->automatic_accept_; }

bool UserConfiguration::get_use_default_path() const { return this->use_default_path_; }

bool UserConfiguration::get_is_public() const { return this->is_public_; }
