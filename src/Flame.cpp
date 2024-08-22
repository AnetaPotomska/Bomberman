#include "Flame.h"

#include "Object.h"
#include "Position.h"

Flame::Flame(const Position & pos, int id) :
    Object(pos, 'x'),
    playerId(id) {}

int Flame::getId() const {
    return playerId;
}