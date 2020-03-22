//
// Created by thallock on 3/10/20.
//

#ifndef PLAYING_SERVER_GAME_MESSAGES_H
#define PLAYING_SERVER_GAME_MESSAGES_H


#include "../../common/state/definitions.h"
#include "../../common/state/state_manipulator.h"

#include "../../common/network/ConnectionContext.h"
#include "../../common/state/LocationManager.h"

#include "../../common/state/manipulation_classes.h"
#include "../../common/network/GameResult.h"

namespace aod::server::message {

void sendCreateEntity(ConnectionContext *context, EntityId entityId, aod::common::state::EntityCreationParams *params);
void sendApplyDamage(ConnectionContext *context, EntityId entityId, DamageInstance& instance, aod::common::state::DamageApplicationResult& result);
void sendDeleteEntity(ConnectionContext *context, EntityId entityId, double newHealth, bool dies);

void sendChangeEntityMovement(ConnectionContext *context, EntityId entityId, Movement& movement);
void sendEntityChangedAction(ConnectionContext *context, EntityId entityId, std::shared_ptr<aod::common::action::Action> action);

void sendGameTime(ConnectionContext *context, GameTime gameTime, std::chrono::high_resolution_clock::time_point& tickTime);

void sendProjectileLaunch(ConnectionContext *context, WeaponFire& fire);
void sendProjectileUpdate(ConnectionContext *context, ProjectileId id, int projectileNum);
void sendProjectileDestroy(ConnectionContext *context, ProjectileId id);

void sendGameOver(ConnectionContext& context, GameResult& result);

void sendRequestAiInput(ConnectionContext& context);


void parseRequestAction(JsonReader& reader, EntityId& entityId, std::shared_ptr<aod::common::action::Action>&);

}


#endif //PLAYING_SERVER_GAME_MESSAGES_H
