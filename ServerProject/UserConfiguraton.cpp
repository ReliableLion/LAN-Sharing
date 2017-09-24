#include "UserConfiguration.h"
#include <string>

UserConfig::UserConfig() {}

void UserConfig::setAutomaticAccept(bool value) { 
	
	//TODO: in this section i will add the automatic accept taken from the interface and place it into a configuration file and in the userConfig instance 
	this->AutomaticAccept = value; 
}

void UserConfig::setDefaultPath(bool useDefaultPath, std::string pathname) {
	//TODO:	add the default path option in the UserConfig instance and add it into the configuration
}

std::string UserConfig::getDefaultPath() { return this->defaultPath; }

bool UserConfig::getAutomaticAccept() { return this->AutomaticAccept; }

bool UserConfig::getUseDefaultPath() { return this->UseDefaultPath; }

bool UserConfig::get_isPublic() { return this->isPublic; }