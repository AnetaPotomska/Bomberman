#pragma once

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

/**
 * @brief When player will found this booster he will get + 1 life.
 * AddLife booster is represented by L symbol. 
*/
class AddLife : public Booster {
    public:
        AddLife(const Position & pos);
        /**
         * @brief Add + 1 life to @param stats of player and call refresh on it.
        */
        virtual void useBonus(Statistics & stats) const  ;
};