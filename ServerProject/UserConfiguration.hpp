#pragma once
#include <fstream>
#include <string>

/*
	this class is the main interface between the UI and the system 
*/
class UserConfiguration {
private:
	bool isPublic;
	bool AutomaticAccept;
	bool UseDefaultPath;

	std::string defaultPath;
	std::string filePath;
	std::string fileName;

	// the use of the configuration file is limited to a test of the system
	std::ifstream writeConfigFile;
	std::ofstream readConfiguFile;

	//TODO: create  a link to communicate with the UI of the application
public:
	UserConfiguration();
	void setAutomaticAccept(bool value);
	void setDefaultPath(bool useDefaultPath, std::string pathname);
	std::string getDefaultPath();
	std::string getFilePath();
	bool getAutomaticAccept();
	bool getUseDefaultPath();
	bool get_isPublic();
}; 
