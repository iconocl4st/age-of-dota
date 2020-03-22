//
// Created by thallock on 3/17/20.
//

#include "ServerSettings.h"

#include <boost/property_tree/json_parser.hpp>

void loadSettings(const std::string &filepath, ServerSettings &settings) {
	boost::property_tree::ptree settingsTree;
	boost::property_tree::read_json(filepath, settingsTree);

	auto outputDirectory = settingsTree.get_optional<std::string>("output-directory");
	if (outputDirectory) {
		settings.outputDirectory = outputDirectory.value();
	}
	auto port = settingsTree.get_optional<int>("port");
	if (port) {
		settings.serverPort = port.value();
	}
	auto numThreads = settingsTree.get_optional<int>("number-of-threads");
	if (numThreads) {
		settings.numThreads = numThreads.value();
	}
	auto condaExecutable = settingsTree.get_optional<std::string>("conda-executable");
	if (condaExecutable) {
		settings.condaExecutable = condaExecutable.value();
	}
	auto condaEnvironmentName = settingsTree.get_optional<std::string>("conda-environment-name");
	if (condaEnvironmentName) {
		settings.condaEnvironmentName = condaEnvironmentName.value();
	}
	auto aiWorkingDirectory = settingsTree.get_optional<std::string>("ai-working-directory");
	if (aiWorkingDirectory) {
		settings.aiWorkingDirectory = aiWorkingDirectory.value();
	}
	auto aiScript = settingsTree.get_optional<std::string>("ai-script");
	if (aiScript) {
		settings.aiScript = aiScript.value();
	}
}

