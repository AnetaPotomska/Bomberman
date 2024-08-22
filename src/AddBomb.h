#pragma once

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

/**
 * @brief When player will found this booster he will get + 1 bomb.
 * AddBomb booster is represented by B symbol.
*/
class AddBomb : public Booster {
    public:
        AddBomb(const Position & pos);
        /**
         * @brief Add + 1 bomb to @param stats of player and call refresh on it.
        */
        virtual void useBonus(Statistics & stats) const;
};