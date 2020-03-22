//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_CLIENT_MESSAGES_H
#define PLAYING_CLIENT_MESSAGES_H



#include "../../common/network/ConnectionContext.h"
#include "../../common/json/JsonReader.h"
#include "../../common/state/Point.h"
#include "../../common/state/definitions.h"

#include "../../common/state/CommonState.h"
#include "../../common/state/LocationManager.h"
#include "../../common/network/GameResult.h"
#include "../../../ui/src/lobbybrowserconnection.h"

namespace aod::client::common::network {

void sendRefreshLobbiesRequest(ConnectionContext &connection);
void joinLobby(ConnectionContext &connection, bool isHuman, const std::string& lobby, int playerNumber);
void setReady(ConnectionContext &connection, bool isReady);

void requestAction(ConnectionContext& connection, EntityId entityId, std::shared_ptr<aod::common::action::Action> action);
void sendAiActionsSent(ConnectionContext& context);




void parseLobbyConnection(JsonReader& reader, LobbyBrowserConnection&);

EntityId parseCreateEntityBody(JsonReader &reader, aod::common::state::CommonState& sharedState);
void parseEntityReceivedDamage(JsonReader& reader, aod::common::state::CommonState&  sharedState);
void parseDeleteEntity(JsonReader &reader, aod::common::state::CommonState& sharedState);

EntityId parseActionChanged(JsonReader &reader, aod::common::state::CommonState &sharedState);
EntityId parseMovementChanged(JsonReader &reader, aod::common::state::CommonState &sharedState);

void parseProjectileLaunch(JsonReader &reader, aod::common::state::CommonState &sharedState);
void parseProjectileUpdate(JsonReader& reader, aod::common::state::CommonState&  sharedState);
void parseProjectileDestroy(JsonReader& reader, aod::common::state::CommonState&  sharedState);

void parseGameTime(JsonReader &reader, aod::common::state::CommonState &sharedState);

void parseGameEnd(JsonReader &reader, GameResult& result);

}

#endif //PLAYING_CLIENT_MESSAGES_H
