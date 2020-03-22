//
// Created by thallock on 2/14/20.
//

#include "ServerContext.h"

#include "../network/ServerMessageHandler.h"
#include "../network/ServerGameHandler.h"
#include "../sim/Engine.h"
#include "../sim/generate_map.h"
#include "../../common/json/JsonTextReader.h"
#include "../../common/json/JsonTextWriter.h"
#include "../../common/json/JsonSplitWriter.h"
#include "../../common/network/CloseOnExit.h"
#include "../../common/network/PlayerLaunchOptions.h"
#include "../network/LobbyHandler.h"
#include "../../common/network/DefaultMessageHandler.h"
#include "../../common/network/send_noop.h"
#include "../../common/network/handle_messages.h"
#include "ServerSettings.h"

#include <thread>
#include <fstream>


namespace aod::server::app {

void ServerContext::handleConnection(int socket, int connectionNumber) {
	CloseOnExit coe{socket};

	std::ostringstream debug_file_path;
	debug_file_path << settings.outputDirectory << "/server_debug_";
	debug_file_path << connectionNumber;
	debug_file_path << ".json";

	std::ofstream debugFile;
	debugFile.open(debug_file_path.str());

	SocketOutputBufferWrapper socketBuffer{socket};
	TextJsonWriter socketWriter{socketBuffer};
	StreamOutputBufferWrapper streamBuffer{debugFile};
	TextJsonWriter streamWriter{streamBuffer};
	JsonSplitWriter splitWriter{socketWriter, streamWriter};

	std::ostringstream connectionName;
	connectionName << "Connection #" << connectionNumber;

	ConnectionContext context(connectionName.str(), splitWriter);
	{
		std::unique_lock<std::recursive_mutex> lock{connectionsMutex};
		connections.push_back(&context);
		closingCv.notify_all();
	}

	sendNoop(context);

	std::list<MessageHandler *>  handlers;
	DefaultMessageHandler dh; handlers.push_back(&dh);
	ServerMessageHandler sh{*this}; handlers.push_back(&sh);
	LobbyHandler lh{*this}; handlers.push_back(&lh);
	ServerGameHandler gh{*this}; handlers.push_back(&gh);

	SocketInputBufferWrapper readBuffer{socket};
	TextJsonReader reader{readBuffer, "server-reader"};
	MessageHandlingArgs handlingContext{reader, context, logger};
	while (!handlingContext.stopHandlingMessages) {
		handleNextMessage(handlers, handlingContext);
	}

	{
		std::unique_lock<std::recursive_mutex> lock{connectionsMutex};
		connections.remove(&context);
		closingCv.notify_all();
	}

	close(socket);
}

void runServer() {
	ServerSettings settings;
	loadSettings("settings/server_settings.json", settings);

	std::random_device seeder;
	ServerContext serverContext{settings};

	BOOST_LOG_SEV(serverContext.logger, NORMAL) << "Launching server.";

	std::ostringstream messages_path;
	messages_path << settings.outputDirectory << "/message_types.json";
	aod::common::message::exportMessages(messages_path.str());
	BOOST_LOG_SEV(serverContext.logger, DEBUG) << "Message types written.";


	std::thread engineLoop{[&serverContext]() -> void { runEngineLoop(serverContext); }};

	serverContext.runServerSocketLoop(settings.serverPort);

	engineLoop.join();

	BOOST_LOG_SEV(serverContext.logger, NORMAL) << "Server shutting down.";
}

void ServerContext::createLobby(const std::string &basicString) {
	std::unique_lock<std::recursive_mutex> lock{lobbiesMutex};
	lobbies.push_back(std::make_shared<Lobby>(logger, settings, basicString));
}

std::shared_ptr<Lobby> ServerContext::findLobby(const std::string &lobbyName) {
	std::unique_lock<std::recursive_mutex> lock{lobbiesMutex};
	for (auto &lobby : lobbies) {
		if (lobby->name == lobbyName) {
			return lobby;
		}
	}
	return nullptr;
}

void ServerContext::removeLobby(const std::string& lobbyName) {
	std::unique_lock<std::recursive_mutex> lock{lobbiesMutex};
	lobbies.remove_if([&lobbyName](auto l) -> bool { return l->name == lobbyName; });
}

void ServerContext::removeGame(const std::string& uuid) {
	std::unique_lock<std::recursive_mutex> lock{gamesMutex};
	games.remove_if([&uuid](auto g) -> bool { return g->uuid == uuid; });
	closingCv.notify_all();

	BOOST_LOG_SEV(logger, NORMAL) << "Removed game " << uuid;
}


void ServerContext::waitForGamesToEnd() {
	std::unique_lock<std::recursive_mutex> lock{gamesMutex};
	closingCv.wait(lock, [this]() -> bool {
		std::unique_lock<std::recursive_mutex> lock{connectionsMutex};
		return games.empty() && connections.empty();
	});
	BOOST_LOG_SEV(logger, NORMAL) << "All games have been closed";
}

std::shared_ptr<ServerGameState> ServerContext::getGame(const std::string &gameUuid) {
	std::unique_lock<std::recursive_mutex> lock{gamesMutex};
	for (auto &game : games) {
		if (game->uuid == gameUuid) {
			return game;
		}
	}
	return nullptr;
}


void ServerContext::debug(JsonWriter &writer) {
	writer.writeBeginObject();

	writeAddress(this, writer);

	writer.writeBoolean("accepting", accepting);
	writer.writeInt("number-of-connections", connections.size());
	writer.writeInt("current-connection-number", currentConnectionNumber);

	{
		std::unique_lock<std::recursive_mutex> lock{connectionsMutex};
		writer.writeBeginArray("connections");
		for (auto &cc : connections) {
			cc->debug(writer);
		}
		writer.writeEndArray();
	}

	{
		std::unique_lock<std::recursive_mutex> lock{lobbiesMutex};
		writer.writeBeginArray("lobbies");
		for (auto &cc : lobbies) {
			cc->debug(writer);
		}
		writer.writeEndArray();
	}


	{
		std::unique_lock<std::recursive_mutex> lock{gamesMutex};
		writer.writeBeginArray("games");
		for (auto &cc : games) {
			cc->debug(writer);
		}
		writer.writeEndArray();
	}

	writer.writeEndObject();
}


void sendLaunch(ConnectionContext *context, PlayerLaunchOptions &options) {
	std::unique_lock<std::recursive_mutex> lock{context->writeLock};
	context->writer.writeBeginObject();
	context->writer.writeInt("message-type", aod::common::message::LAUNCHED);
	context->writer.writeKey("launch-options");
	options.send(&context->writer);
	context->writer.writeEndObject();
	context->writer.flush();
}

std::shared_ptr<ServerGameState> ServerContext::createGame(LaunchOptions &options) {
	std::string uuid = numberGenerator.nextString(20);
	std::unique_lock<std::recursive_mutex> lock{gamesMutex};
	std::shared_ptr<ServerGameState> ptr = std::make_shared<ServerGameState>(uuid, options);
	games.push_back(ptr);
	return ptr;
}

void ServerContext::launchGame(Lobby *lobby) {
	// Have to think through the locking a little bit more....
	std::shared_ptr<ServerGameState> serverGameState = createGame(lobby->options);
	serverGameState->entityIdGenerator = [this]() -> EntityId { return numberGenerator.nextInt(0, 1000000); };

	PlayerLaunchOptions playerOptions;
	playerOptions.spec = serverGameState->sharedState.specification;
	playerOptions.numPlayers = 1 + lobby->getNumPlayers();
	{
		std::unique_lock<std::recursive_mutex> lock{lobby->mutex};

		if (!lobby->areAllPlayersReady()) {
			// free new server game state...
			throw std::runtime_error("Tried to launch without players being ready");
		}

		for (auto &aiThread : lobby->aiThreads) {
			serverGameState->aiThreads.push_back(aiThread);
		}
		lobby->aiThreads.clear();

		for (auto &connection : lobby->spectators) {
			connection->attributes.erase("lobby");
			serverGameState->spectatorConnections.push_back(connection);
		}
		lobby->spectators.clear();

		int index = 0;
		for (auto &slot : lobby->playerSlots) {
			auto connection = slot->getConnection();
			connection->attributes.erase("lobby");
			serverGameState->playerConnections[++index] = connection;
		}
		lobby->playerSlots.clear();

		removeLobby(lobby->name);
	}

	playerOptions.playerNumber = -1;
	playerOptions.isSpectator = true;
	for (auto &connection : serverGameState->spectatorConnections) {
		connection->attributes["game"] = serverGameState->uuid;
		sendLaunch(connection, playerOptions);
	}

	playerOptions.isSpectator = false;
	for (auto &entry : serverGameState->playerConnections) {
		entry.second->attributes["game"] = serverGameState->uuid;
		playerOptions.playerNumber = entry.first;
		sendLaunch(entry.second, playerOptions);
	}

	randomly_generate_map(numberGenerator, serverGameState.get());

	serverGameState->requestAiActions();
	serverGameState->flushAll();

	serverGameState->isTicking = true;
	closingCv.notify_all();
	BOOST_LOG_SEV(logger, NORMAL) << "Created game " << serverGameState->uuid;
}

void ServerContext::cancelLobby(Lobby *lobby) {
	throw std::runtime_error("Implement me!!!");
}

int ServerContext::getAndIncrementCurrentConnectionNumber() {
	std::unique_lock<std::recursive_mutex> lock{connectionsMutex};
	return currentConnectionNumber++;
}


static unsigned int getRandomSeed() {
	std::random_device seeder;
	return seeder();
}

ServerContext::ServerContext(ServerSettings& settings) :
	settings{settings},
	connectionPool{(size_t) settings.numThreads},
	numberGenerator{getRandomSeed()}
{

}

}
