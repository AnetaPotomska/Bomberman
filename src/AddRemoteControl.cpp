#include "AddRemoteControl.h"

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

AddRemoteControl::AddRemoteControl(const Position & pos) :
    Booster(pos, 'R') {}

void AddRemoteControl::useBonus(Statistics & stats) const {
    stats.addRemoteControl();
    updateStatsGeneral(stats);
}