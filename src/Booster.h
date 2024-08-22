#pragma once

#include "Object.h"
#include "Position.h"
#include "Statistics.h"

/**
 * There are 6 possible boosters and this class is parent for them:
 * D: increase bomb distance 
 * S: speed of players move - human player will have more time to think
 * R: add remote control - player can detonate all of his bombs
 * L: increase num of lives 
 * B: increase num of bombs 
 * P: bomb pass - player can go on his bombs
*/
class Booster : public Object {
    public:
        Booster(const Position & pos, char symbol);
        /**
         * @brief It will update @param stats of player.
        */
        virtual void useBonus(Statistics & stats) const = 0;
    protected:
        /**
         * @brief Refresh table of stats.
        */
        void updateStatsGeneral(Statistics & stats) const;
};