#pragma once

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

/**
 * @brief When player will found this booster he will get + 1 on flames distance when bomb explodes. 
 * AddFlames booster is represented by F symbol.
*/
class AddFlames : public Booster {
    public:
        AddFlames(const Position & pos);
        /**
         * @brief Add + 1 to bomb flames to @param stats of player and call refresh on it.
        */
        virtual void useBonus(Statistics & stats) const  ;
};