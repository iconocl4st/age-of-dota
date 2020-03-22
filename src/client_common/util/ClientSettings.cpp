//
// Created by thallock on 3/18/20.
//

#include "ClientSettings.h"

#include <boost/property_tree/json_parser.hpp>


namespace aod::client::common {

void loadSettings(const std::string& filepath, ClientSettings& settings) {
	boost::property_tree::ptree settingsTree;
	boost::property_tree::read_json(filepath, settingsTree);

	auto logPath = settingsTree.get_optional<std::string>("output-directory");
	if (logPath) {
		settings.outputDirectory = logPath.value();
	}
	auto serverAddress = settingsTree.get_optional<std::string>("server-address");
	if (serverAddress) {
		settings.serverAddress = serverAddress.value();
	}
	auto port = settingsTree.get_optional<int>("port");
	if (port) {
		settings.port = port.value();
	}
	auto numThreads = settingsTree.get_optional<int>("number-of-threads");
	if (numThreads) {
//		settings.numThreads = numThreads.value();
	}
}

}
