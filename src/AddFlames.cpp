#include "AddFlames.h"

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

AddFlames::AddFlames(const Position & pos) :
    Booster(pos, 'F') {}

void AddFlames::useBonus(Statistics & stats) const {
    stats.updateFlames(1);
    updateStatsGeneral(stats);
}