#pragma once

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

/**
 * @brief When player will found this booster he will get + 1 speed.
 * This effect only affects human players. It will allow them to have more time on their turn.
 * AddSpeed booster is represented by S symbol. 
*/
class AddSpeed : public Booster {
    public:
        AddSpeed(const Position & pos);
        /**
         * @brief Add + 1 speed to @param stats of player and call refresh on it.
        */
        virtual void useBonus(Statistics & stats) const  ;
};