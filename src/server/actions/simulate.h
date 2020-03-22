//
// Created by thallock on 3/12/20.
//

#ifndef PLAYING_SIMULATE_H
#define PLAYING_SIMULATE_H

#include "../sim/EntitySimInfo.h"

namespace aod::server::engine::simulate {

void move(aod::common::action::Move *action, EntitySimInfo& simInfo);

void attack(aod::common::action::Attack *action, EntitySimInfo& simInfo);

void idle(aod::common::action::Idle *action, EntitySimInfo& simInfo);

void collect(aod::common::action::Collect *action, EntitySimInfo& simInfo);

void deposit(aod::common::action::Deposit *action, EntitySimInfo& simInfo);

void strafe(aod::common::action::Strafe *action, EntitySimInfo& simInfo);

}


#endif //PLAYING_SIMULATE_H
