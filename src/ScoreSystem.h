#pragma once

#include "../libs/json.hpp"

/**
 * @brief Stores what action holds how much score.
*/
struct ScoreSystem {
    public:
        ScoreSystem(const nlohmann::json & scoreSystem);
        int killingOtherPlayer = 10;
        int activatingAnotherBomb = 5;
        int pickingUpBonus = 3;
        int destroyingWall = 2;
        int placingBomb = 1;
        int killingSelf = -1;
    private:
        /**
         * @brief He will go through @param scoreSystem and saves score to corresponding action.
        */
        void parseScoreSystemFromJson(const nlohmann::json & scoreSystem);
};