#ifndef USER_CONFIGURATION_H 
#define USER_CONFIGURATION_H 

#include <fstream>
#include <string>

/*
	this class is contain all the information about the application config
	such automatic confirm, default path and others settings used in the application 
*/
class UserConfig {
private:
	bool isPublic;
	bool AutomaticAccept;
	bool UseDefaultPath;
	std::string defaultPath;
	std::ifstream writeConfigFile;
	std::ofstream readConfigureFile;
	std::string fileName;

	//TODO: create  a link to communicate with the UI of the application
public:
	UserConfig();
	void setAutomaticAccept(bool value);
	void setDefaultPath(bool useDefaultPath, std::string pathname);
	std::string getDefaultPath();
	bool getAutomaticAccept();
	bool getUseDefaultPath();
	bool get_isPublic();
};


#endif // USER_CONFIGURATION_H 
