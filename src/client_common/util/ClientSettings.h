//
// Created by thallock on 3/18/20.
//

#ifndef PLAYING_CLIENTSETTINGS_H
#define PLAYING_CLIENTSETTINGS_H

#include <string>

namespace aod::client::common {

class ClientSettings {
public:
	std::string serverAddress{"127.0.0.1"};
	std::string outputDirectory{"./output"};
	int port{8083};
};

void loadSettings(const std::string& filepath, ClientSettings& settings);

}


#endif //PLAYING_CLIENTSETTINGS_H
