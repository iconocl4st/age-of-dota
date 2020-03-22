//
// Created by thallock on 3/7/20.
//

#include <fstream>

#include "Engine.h"

#include "../app/ServerContext.h"
#include "../../common/util/Timer.h"
#include "../actions/simulate.h"
#include "EntitySimInfo.h"
#include "../network/server_game_messages.h"
#include "../../common/state/Entity.h"
#include "../../common/network/send_noop.h"

static void validateAction(std::shared_ptr<ServerGameState>& pState, std::shared_ptr<aod::common::action::Action>& action) {
	switch (action->getType()) {
		case aod::common::action::MOVE: {
			auto move = (aod::common::action::Move *) action.get();
			move->normalizeAngle();
			move->placeWithinBounds(
				pState->sharedState.specification->width,
				pState->sharedState.specification->height
			);
			break;
		}
	}
}

static void updateRequests(std::shared_ptr<ServerGameState> pState) {
	std::unique_lock<std::recursive_mutex> lock{pState->incomingRequests.mutex};

	for (auto& it : pState->incomingRequests.requestedActions) {
		// TODO: timestamp...

		validateAction(pState, it.second);
		aod::common::state::setAction(pState->sharedState, it.first, it.second);
		pState->broadcast([&it](ConnectionContext &c) -> void {
			aod::server::message::sendEntityChangedAction(&c, it.first, it.second);
		});
	}
	pState->incomingRequests.requestedActions.clear();
}

static void processActions(ServerGameState *gState, TickInfo& tickInfo, StateChange& delta) {
    EntitySimInfo simInfo{gState, tickInfo, delta};

    for (auto & entityId : gState->sharedState.entityIds) {
        // lock...
        auto it = gState->sharedState.actionManager.find(entityId);
        if (it == gState->sharedState.actionManager.end()) {
        	continue;
        }

        auto& action = it->second;
        simInfo.entity = entityId;
        switch (action->getType()) {
        	case aod::common::action::IDLE:
				aod::server::engine::simulate::idle(
					(aod::common::action::Idle *) action.get(), simInfo);
				break;
        	case aod::common::action::MOVE:
				aod::server::engine::simulate::move(
					(aod::common::action::Move *) action.get(), simInfo);
				break;
        	case aod::common::action::COLLECT:
				aod::server::engine::simulate::collect(
					(aod::common::action::Collect *) action.get(), simInfo);
				break;
        	case aod::common::action::DEPOSIT:
				aod::server::engine::simulate::deposit(
					(aod::common::action::Deposit *) action.get(), simInfo);
				break;
        	case aod::common::action::ATTACK:
				aod::server::engine::simulate::attack(
					(aod::common::action::Attack *) action.get(), simInfo);
				break;
			case aod::common::action::STRAFE:
				aod::server::engine::simulate::strafe(
					(aod::common::action::Strafe *) action.get(), simInfo);
				break;
        	default:
        		throw std::runtime_error("Unrecognized action");
		}
    }
}


static void processProjectiles(ServerGameState *gState, TickInfo& tickInfo, StateChange& delta) {
	std::set<ProjectileId> toDelete;
	auto & state = gState->sharedState;

	for (auto & it : state.attacks) {
		auto& projectile = it.second;
		double innerRadius = projectile.projectileSpeed * (tickInfo.tickBegin - projectile.launchTime);
		double outerRadius = projectile.projectileSpeed * (tickInfo.getTickEnd() - projectile.launchTime);
		auto projectileId = projectile.id;

		for (int projectileNum = 0; projectileNum < projectile.numProjectiles; projectileNum++) {
			if (projectile.isProjectileSunk(projectileNum)) {
				continue;
			}

			auto theta = (float) projectile.getOrientation(projectileNum);

			double x1 = projectile.origin.x + innerRadius * cosf(theta);
			double y1 = projectile.origin.y + innerRadius * sinf(theta);
			double x2 = projectile.origin.x + outerRadius * cosf(theta);
			double y2 = projectile.origin.y + outerRadius * sinf(theta);

			double xmin = std::min(x1, x2);
			double ymin = std::min(y1, y2);
			double xmax = std::max(x1, x2);
			double ymax = std::max(y1, y2);

			Point bMin{xmin, ymin};
			Point bMax{xmax, ymax};

			Point p1{x1, y1};
			Point p2{x2, y2};

			state.locationManager.visitEntities(state, bMin, bMax, [p1, p2, &projectile, gState, projectileNum](EntityId entityId, const Point& loc) -> void {
				if (projectile.launcher == entityId) {
					return;
				}
				Entity entity{entityId, gState->sharedState};
				if (entity.getType()->occupies->intersectsLine(loc, entity.getOrientation().value(), p1, p2)) {
					gState->sharedState.attacks[projectile.id].sunkProjectiles.insert(projectileNum);
					gState->broadcast([&projectile, projectileNum](ConnectionContext& c) -> void {
						aod::server::message::sendProjectileUpdate(&c, projectile.id, projectileNum);
					});

					DamageInstance instance{
						projectile.launcher,
						projectile.origin,
						projectile.damageType,
						projectile.damage
					};

					aod::common::state::DamageApplicationResult result =
						aod::common::state::apply_damage(gState->sharedState, entityId, instance);
					gState->broadcast([entityId, &instance, &result](ConnectionContext& c) -> void {
						aod::server::message::sendApplyDamage(&c, entityId, instance, result);
					});
				}
			});
		}
		if (innerRadius > projectile.range) {
			gState->broadcast([projectileId](ConnectionContext& c) -> void {
				aod::server::message::sendProjectileDestroy(&c, projectileId);
			});
			toDelete.insert(projectileId);
		}
	}

	for (auto &id : toDelete) {
		state.attacks.erase(id);
	}
}


static bool processTick(ServerGameState *gameState, TickInfo& info) {
    StateChange delta;

    processActions(gameState, info, delta);
    processProjectiles(gameState, info, delta);

    return info.tickBegin > 60;
}

static void countNumEntities(std::shared_ptr<ServerGameState>& gameState, int& maxNumUnits, EntityId& maxPlayer) {
	std::map<Player, int> counts;
	for (auto & it : gameState->sharedState.playerManager) {
		auto countIt = counts.find(it.second);
		int num;
		if (countIt == counts.end()) {
			num = 1;
		} else {
			num = (countIt->second) + 1;
		}
		counts[it.second] = num;
		if (num <= maxNumUnits) {
			continue;
		}
		maxNumUnits = num;
		maxPlayer = it.second;
	}
}

static void closeGame(aod::server::app::ServerContext& serverContext, std::shared_ptr<ServerGameState> gameState) {
	EntityId maxPlayer = 0;
	int maxNumUnits = -1;

	countNumEntities(gameState, maxNumUnits, maxPlayer);

	GameResult result;
	result.winnerPlayerNumber = maxPlayer;
	gameState->broadcast([&result](ConnectionContext& context) ->  void {
		aod::server::message::sendGameOver(context, result);
		sendCloseConnection(context);
	});
	serverContext.removeGame(gameState->uuid);
	gameState->isTicking = false;
}


void runEngineLoop(aod::server::app::ServerContext& serverContext) {
	BOOST_LOG_SEV(serverContext.logger, NORMAL) << "Launching engine loop.";

	Timer timer{std::chrono::milliseconds{50}};

	while (serverContext.isRunning()) {
		std::set<std::shared_ptr<ServerGameState>> toUpdate;
		{
			std::unique_lock<std::recursive_mutex> lock{serverContext.gamesMutex};
			for (auto & state : serverContext.games) {
				if (state->isTicking) {
					toUpdate.insert(state);
				}
			}
		}

		for (auto & state : toUpdate) {
			state->waitUntilAllReady();

			updateRequests(state);

			std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
			GameTime newGameTime = state->sharedState.interpolateTime(currentTime);

			TickInfo info;
			info.tickBegin = state->sharedState.currentTime;
			info.timeDelta = newGameTime - info.tickBegin;

			BOOST_LOG_SEV(serverContext.logger, DEBUG) << "Beginning tick " << info.tickBegin;

			bool gameOver = processTick(state.get(), info);

			state->sharedState.currentTime = newGameTime;
			state->sharedState.lastTickTime  = currentTime;

			state->broadcast([newGameTime, &currentTime](ConnectionContext& context) -> void {
				aod::server::message::sendGameTime(&context, newGameTime, currentTime);
			});

			if (gameOver) {
				closeGame(serverContext, state);
			} else {
				state->requestAiActions();
			}

			state->flushAll();
		}

		{
			std::ofstream debugStream;
			std::ostringstream outFileName;
			outFileName << serverContext.settings.outputDirectory << "current_server_state.json";
			debugStream.open(outFileName.str());
			debugStream << printJsonDebuggable(&serverContext) << std::endl;
		}

		timer.waitForNextTick();
	}
}