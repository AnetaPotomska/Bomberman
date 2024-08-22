#include "AddBombPass.h"

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

AddBombPass::AddBombPass(const Position & pos) :
    Booster(pos, 'P') {}

void AddBombPass::useBonus(Statistics & stats) const {
    stats.addBombPass();
    updateStatsGeneral(stats);
}