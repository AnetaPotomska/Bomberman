#include "Wall.h"

#include "Object.h"
#include "Position.h"

Wall::Wall(const Position & pos, char symbol, bool destroyable) :
    Object(pos, symbol),
    destroyable(destroyable) {}

bool Wall::isDestroyable() const {
    return destroyable;
}