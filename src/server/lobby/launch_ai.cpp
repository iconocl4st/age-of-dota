//
// Created by thallock on 3/8/20.
//

#include "launch_ai.h"

#include <boost/process.hpp>
#include <iostream>
#include <sstream>

void run_ai_script(const ServerSettings& settings, const std::string lobbyName, int playerNumber) {
    boost::process::ipstream pipe_stream;

    boost::filesystem::path condaExe = settings.condaExecutable;
    std::string condaEnvironment = settings.condaEnvironmentName;
    std::string working_directory = settings.aiWorkingDirectory;
    std::string python_file = settings.aiScript;


    std::ostringstream numStream;
    numStream << playerNumber;

    boost::process::child c(
		condaExe,
        "run",
        "-n",
		condaEnvironment,
		"python",
        python_file,
        lobbyName,
        numStream.str(),
        boost::process::env["PYTHON_PATH"] = working_directory,
        boost::process::std_out > pipe_stream
    );

    std::string line;

    std::ostringstream prefixStream;
    prefixStream << "\t\tAI CHILD #" << playerNumber << ": ";
    std::string prefixSting = prefixStream.str();

    while (pipe_stream && std::getline(pipe_stream, line) && !line.empty()) {
        std::cout << prefixSting << line << std::endl;
    }

    c.wait();
}
