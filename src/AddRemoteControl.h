#pragma once

#include "Booster.h"
#include "Position.h"
#include "Statistics.h"

/**
 * @brief When player will found this booster he will get remote control option.
 * Which means he can detonate all his own bombs hwn he likes.
 * AddRemoteControl booster is represented by R symbol. 
*/
class AddRemoteControl : public Booster {
    public:
        AddRemoteControl(const Position & pos);
        /**
         * @brief Add remote control to @param stats of player and call refresh on it.
        */
        virtual void useBonus(Statistics & stats) const  ;
};