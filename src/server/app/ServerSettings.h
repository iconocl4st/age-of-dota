//
// Created by thallock on 2/14/20.
//

#ifndef PLAYING_SERVERSETTINGS_H
#define PLAYING_SERVERSETTINGS_H

#include <string>

class ServerSettings {
public:
	std::string outputDirectory{"./output"};
	int serverPort{8083};
	int numThreads{3};

	std::string condaExecutable;
	std::string condaEnvironmentName;
	std::string aiWorkingDirectory;
	std::string aiScript;
};

void loadSettings(const std::string& filepath, ServerSettings& settings);

#endif //PLAYING_SERVERSETTINGS_H
