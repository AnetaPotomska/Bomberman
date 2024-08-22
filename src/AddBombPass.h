#pragma once

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

/**
 * @brief When player will found this booster he will get bomb pass.
 * Which mean he will be able to go to his own bombs. 
 * AddBombPass booster is represented by P symbol.
*/
class AddBombPass : public Booster {
    public:
        AddBombPass(const Position & pos);
        /**
         * @brief Add bomb pass to @param stats of player and call refresh on it.
        */
        virtual void useBonus(Statistics & stats) const  ;
};