//
// Created by thallock on 2/15/20.
//

#include "LobbyBrowserAdapter.h"
#include "../app/UiClientContext.h"
#include "../../common/network/MessageTypes.h"
#include "../../client_common/network/client_messages.h"


namespace aod::client::ui {


void LobbyBrowserAdapter::refreshLobbies() {
	aod::client::common::network::sendRefreshLobbiesRequest(*context.getConnection());
}

void LobbyBrowserAdapter::connect(const std::string &lobbyName) {
	aod::client::common::network::joinLobby(*context.getConnection(), true, lobbyName, -1);
}

void LobbyBrowserAdapter::setReady(bool ready) {
	aod::client::common::network::setReady(*context.getConnection(), ready);
}


// Could use the rest of these as well...

void LobbyBrowserAdapter::createLobby(const std::string &lobbyName) {
	if (lobbyName.empty())
		return;

	auto connection = context.getConnection();

	{
		std::unique_lock<std::recursive_mutex> lock{connection->writeLock};
		connection->writer.writeBeginObject();
		connection->writer.writeInt("message-type", aod::common::message::CREATE_LOBBY);
		connection->writer.writeString("lobby-name", lobbyName);
		connection->writer.writeEndObject();
		connection->writer.flush();
	}

	refreshLobbies();
}

void LobbyBrowserAdapter::disconnect() {
	auto connection = context.getConnection();

	{
		std::unique_lock<std::recursive_mutex> lock{connection->writeLock};
		connection->writer.writeBeginObject();
		connection->writer.writeInt("message-type", aod::common::message::LEAVE_LOBBY);
		connection->writer.writeEndObject();
		connection->writer.flush();
	}
}

void LobbyBrowserAdapter::launch() {
	auto connection = context.getConnection();

	{
		std::unique_lock<std::recursive_mutex> lock{connection->writeLock};
		connection->writer.writeBeginObject();
		connection->writer.writeInt("message-type", aod::common::message::LAUNCH);
		connection->writer.writeEndObject();
		connection->writer.flush();
	}
}

void LobbyBrowserAdapter::requestStatusUpdate() {
	auto connection = context.getConnection();

	{
		std::unique_lock<std::recursive_mutex> lock{connection->writeLock};
		connection->writer.writeBeginObject();
		connection->writer.writeInt("message-type", aod::common::message::REQUEST_STATUS_UPDATE);
		connection->writer.writeEndObject();
		connection->writer.flush();
	}
}

void LobbyBrowserAdapter::setSpectator(bool b) {
	auto connection = context.getConnection();

	{
		std::unique_lock<std::recursive_mutex> lock{connection->writeLock};
		connection->writer.writeBeginObject();
		connection->writer.writeInt("message-type", aod::common::message::SET_PLAYER_TO_SPECTATOR);
		connection->writer.writeBoolean("spectating", b);
		connection->writer.writeEndObject();
		connection->writer.flush();
	}
}

void LobbyBrowserAdapter::setNumPlayers(int i) {
	auto connection = context.getConnection();

	{
		std::unique_lock<std::recursive_mutex> lock{connection->writeLock};
		connection->writer.writeBeginObject();
		connection->writer.writeInt("message-type", aod::common::message::SET_NUM_LOBBY_PLAYERS);
		connection->writer.writeInt("number-of-players", i);
		connection->writer.writeEndObject();
		connection->writer.flush();
	}
}

void LobbyBrowserAdapter::setPlayerAsAi(int i, bool b) {
	auto connection = context.getConnection();

	{
		std::unique_lock<std::recursive_mutex> lock{connection->writeLock};
		connection->writer.writeBeginObject();
		connection->writer.writeInt("message-type", aod::common::message::SET_PLAYER_TO_AI);
		connection->writer.writeInt("player-number", i);
		connection->writer.writeBoolean("is-ai", b);
		connection->writer.writeEndObject();
		connection->writer.flush();
	}
}

}
