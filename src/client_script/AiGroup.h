//
// Created by thallock on 3/12/20.
//

#ifndef PLAYING_AIGROUP_H
#define PLAYING_AIGROUP_H

#include <string>

#include "../common/state/definitions.h"

class ScriptClientContext;

class AiGroup {
public:
	virtual ~AiGroup() = default;

	virtual const std::string& getName() = 0;
	virtual void addEntity(ScriptClientContext&, EntityId) = 0;
	virtual void removeEntity(EntityId) = 0;
	virtual void assignTasks(ScriptClientContext&) = 0;
};


#endif //PLAYING_AIGROUP_H
