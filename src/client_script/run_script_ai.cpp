//
// Created by thallock on 3/12/20.
//

#include "run_script_ai.h"

#include <fstream>

#include "../common/network/sockets.h"
#include "../common/network/CloseOnExit.h"
#include "../common/network/BufferWrapper.h"
#include "../common/json/JsonTextWriter.h"
#include "../common/json/JsonSplitWriter.h"
#include "../common/network/ConnectionContext.h"
#include "../common/network/send_noop.h"
#include "../common/json/JsonTextReader.h"
#include "../client_common/network/client_messages.h"
#include "../common/network/PlayerLaunchOptions.h"
#include "../client_common/util/ClientSettings.h"
#include "navigate_lobby.h"
#include "ScriptClientContext.h"
#include "handle_game_messages.h"


int runScriptAi(const std::string &lobbyName, int playerNumber) {
	Logger logger;

	aod::client::common::ClientSettings settings;
	loadSettings("settings/client_settings.json", settings);

	BOOST_LOG_SEV(logger, NORMAL) << "Script client running.";

	int socket = createClientSocket(settings.serverAddress, settings.port);
	BOOST_LOG_SEV(logger, NORMAL) << "Script client connected.";
	CloseOnExit cls{socket};

	std::ostringstream debugPath;
	debugPath << settings.outputDirectory << "/script_client_";
	debugPath << lobbyName << "_" << playerNumber  << ".json";
	std::ofstream debugFile;
	debugFile.open(debugPath.str());

	SocketOutputBufferWrapper socketBuffer{socket};
	TextJsonWriter socketWriter{socketBuffer};
	StreamOutputBufferWrapper streamBuffer{debugFile};
	TextJsonWriter streamWriter{streamBuffer};
	JsonSplitWriter writer{socketWriter, streamWriter};

	ConnectionContext connectionContext{"Script client", writer};

	sendNoop(connectionContext);
	aod::client::common::network::sendRefreshLobbiesRequest(connectionContext);

	SocketInputBufferWrapper readBuffer{socket};
	TextJsonReader reader{readBuffer, "client_ui-reader"};

	PlayerLaunchOptions launchOptions;
	if (!navigate_lobby(lobbyName, playerNumber, connectionContext, reader, launchOptions, logger)) {
		return -1;
	}

	std::random_device seeder;
	ScriptClientContext clientContext{seeder(), connectionContext, launchOptions};

	// do the launch things...

	return handleGameMessages(reader, clientContext);
}
