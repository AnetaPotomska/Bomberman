#include "AddSpeed.h"

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

AddSpeed::AddSpeed(const Position & pos) :
    Booster(pos, 'S') {}

void AddSpeed::useBonus(Statistics & stats) const {
    stats.updateSpeed(1);
    updateStatsGeneral(stats);
}