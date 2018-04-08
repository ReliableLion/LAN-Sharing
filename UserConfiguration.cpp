#include "UserConfiguration.hpp"

UserConfiguration::UserConfiguration() {}

void UserConfiguration::setAutomaticAccept(bool value) {

	//TODO: in this section i will add the automatic accept taken from the interface and place it into a configuration file and in the UserConfiguration instance 
	this->AutomaticAccept = value;
}

void UserConfiguration::setDefaultPath(bool useDefaultPath, std::string pathname) {
	//TODO:	add the default path option in the UserConfiguration instance and add it into the configuration
}

std::string UserConfiguration::getFilePath() { return this->filePath; }

std::string UserConfiguration::getDefaultPath() { return this->defaultPath; }

bool UserConfiguration::getAutomaticAccept() { return this->AutomaticAccept; }

bool UserConfiguration::getUseDefaultPath() { return this->UseDefaultPath; }

bool UserConfiguration::get_isPublic() { return this->isPublic; }