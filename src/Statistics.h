#pragma once

#include "InfoTable.h"
#include "../libs/json.hpp"

/**
 * @brief Holds all statistics of player and info table where he prints these stats.
*/
class Statistics {
    public:
        Statistics() = default;
        Statistics(int id, const nlohmann::json & playersStats);
        void updateLives(int add);
        void updateBombs(int add);
        void updateScore(int add);
        void updateFlames(int add);
        void updateSpeed(int add);
        void addBomb(int add);
        void addRemoteControl();
        void addBombPass();
        bool isAlive() const;
        void refreshInfoTable();
        void clearInfoTable();

        int id;
        int score = 0;
        int speed = 3;
        int lives = 1;
        int bombsAvailable = 1;
        int bombsPlaced = 0;
        int flamesPower = 1;
        int counterOnBombs = 8;
        bool hasRemoteControl = false;
        bool hasBombPass = false;
    private:
        /**
         * @brief Goes through every entry in @param playersStats and update corresponding variable with it.
        */
        void parseStatsFromJson(const nlohmann::json & playersStats);
        InfoTable infoTable;
};