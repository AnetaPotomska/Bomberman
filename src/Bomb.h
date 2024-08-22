#pragma once

#include <map>

#include "Object.h"
#include "Flame.h"
#include "Position.h"
#include "Wall.h"

/**
 * @brief Represents bomb.
 * Bomb is represented by X symbol.
*/
class Bomb : public Object {
    public:
        Bomb() = default;
        Bomb(const Position & pos, int counter, int playerId, int flamesPower);
        /**
         * @brief If counter is 0 bomb is about to explode.
        */
        bool isGoingToExplode();
        void updateCountdown(int add);
        /**
         * @return Positions where would flame go.
         * When flame runs into wall it will lights this wall in fire, but won't continue that direction.
         * Destroyable wall are destroyed by flame.
        */
        std::map<Position, Flame> explode(const std::map<Position, Wall> & walls);
        int getId() const;
        int getCounter() const;
    private:
        int counter; // how many turns until it detonates 
        int playerId; // who placed the bomb
        int flamesPower; // how big flames it will create
};