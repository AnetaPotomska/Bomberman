#pragma once

#include <ncurses.h>
#include <memory>
#include <map>
#include <string>

#define WIDTH_INFO_WIN 14
#define HEIGHT_INFO_WIN 11

/**
 * @brief Window for rendering players statistics.
*/
class InfoTable {
    public:
        InfoTable();
        /**
         * @brief It will create window. 
         * It also determines on @param playerId where to put this window.
         * For example palyer 0 has his infotable in upper left corner and so on as you would read:
         * from left to right, from up to down.
        */
        void initWin(int playerId);
        void clear();
        void refreshWin();
        /**
         * @brief Updates every data this tabs hold (exclude from players id).
        */
        void updateEverything(int score, int lives, int bombsAvailable, int flamesPower, int hasRemoteControl, int speed, int hasBombPass);
        void printData();
    private:
        /**
         * @brief This will print whole table. 
         * That means which this table belongs to and containing data.
        */
        void printInit();
        std::shared_ptr<WINDOW> infoWin;
        std::map<std::string, int> data; // Currently there are 7 records to show: score, bomb count, lives, speed, flames range, if player has detonator and if player has bomb pass.
        int playerId;
};