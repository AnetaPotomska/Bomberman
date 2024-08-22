#include "AddLife.h"

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

AddLife::AddLife(const Position & pos) :
    Booster(pos, 'L') {}

void AddLife::useBonus(Statistics & stats) const {
    stats.updateLives(1);
    updateStatsGeneral(stats);
}