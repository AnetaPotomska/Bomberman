#include "Bomb.h"

#include <map>

#include "Object.h"
#include "Flame.h"
#include "Position.h"
#include "Wall.h"

Bomb::Bomb(const Position & pos, int counter, int playerId, int flamesPower) :
    Object(pos, 'X'),
    counter(counter),
    playerId(playerId),
    flamesPower(flamesPower) {}

bool Bomb::isGoingToExplode() {
    return counter == 0;
}

void Bomb::updateCountdown(int add) {
    counter += add;
}

std::map<Position, Flame> Bomb::explode(const std::map<Position, Wall> & walls) {
    std::map<Position, Flame> whereFlames;
    bool left = true;
    bool right = true;
    bool down = true;
    bool up = true;
    for(int i = 0; i <= flamesPower; i++) {
        Position posLeft = Position(pos.y, pos.x - i);
        Position posRight = Position(pos.y, pos.x + i);
        Position posUp = Position(pos.y - i, pos.x);
        Position posDown = Position(pos.y + i, pos.x);
        if(left)
            whereFlames[posLeft] = (Flame(posLeft, playerId));
        if(right)
            whereFlames[posRight] = (Flame(posRight, playerId));
        if(up)
            whereFlames[posUp] = (Flame(posUp, playerId));
        if(down)
            whereFlames[posDown] = (Flame(posDown, playerId));
        if(walls.count(posLeft)) {
            left = false;
        }
        if(walls.count(posRight)) {
            right = false;
        }
        if(walls.count(posUp)) {
            up = false;
        }
        if(walls.count(posDown)) {
            down = false;
        }
    }
    return whereFlames;
}

int Bomb::getId() const {
    return playerId;
}

int Bomb::getCounter() const {
    return counter;
}