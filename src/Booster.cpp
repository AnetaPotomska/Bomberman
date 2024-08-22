#include "Booster.h"

#include "Object.h"
#include "Position.h"
#include "Statistics.h"

Booster::Booster(const Position & pos, char symbol) :
    Object(pos, symbol) {}

void Booster::updateStatsGeneral(Statistics & stats) const {
    stats.refreshInfoTable();
}