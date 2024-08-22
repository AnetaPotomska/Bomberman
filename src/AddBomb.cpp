#include "AddBomb.h"

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

AddBomb::AddBomb(const Position & pos) :
    Booster(pos, 'B') {}

void AddBomb::useBonus(Statistics & stats) const {
    stats.addBomb(1);
    updateStatsGeneral(stats);
}