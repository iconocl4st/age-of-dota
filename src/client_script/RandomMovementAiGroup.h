//
// Created by thallock on 3/12/20.
//

#ifndef PLAYING_RANDOMMOVEMENTAIGROUP_H
#define PLAYING_RANDOMMOVEMENTAIGROUP_H

#include <set>

#include "AiGroup.h"

void assignMovement(ScriptClientContext& cntxt, EntityId entityId);

class RandomMovementAiGroup : public AiGroup {
private:
	std::set<EntityId> entities;
	std::string name{"randomly-move"};
public:
	virtual ~RandomMovementAiGroup() = default;

	const std::string& getName() { return name; }
	void addEntity(ScriptClientContext& cntxt, EntityId entity);
	void removeEntity(EntityId);
	void assignTasks(ScriptClientContext& cntxt) {};
};


#endif //PLAYING_RANDOMMOVEMENTAIGROUP_H
