#pragma once

#include <string>
#include <memory>
#include <ncurses.h>
#include <string>

#include "OneStringWin.h"
#include "../libs/json.hpp"

#define HEIGHT_SB_WIN 20
#define WIDTH_SB_WIN 40
#define MAX_RECORDS HEIGHT_SB_WIN - 4

/**
 * @brief Manages scoreboards. Reading them and writing to them.
 * ScoreBoardManager creates its own window where it displays the results when reading.
 * I'm disallowing player names longer than 14 characters to make this window at least always the same width 
 * (possible scores should not exceed more than 10 characters).
*/
class ScoreBoardManager {
    public:
        ScoreBoardManager() = default;
        ScoreBoardManager(int gameMode, const std::string & levelName, const nlohmann::json & scoreboardsDirs);
        /**
         * @brief Initilize window. Call startWin for init print and then loadSB.
        */
        void print();
        /**
         * @brief It will read all records in scoreboard and saves it to memory.
         * Then it will go through it if there is same name (and updates score if new score is bigger).
         * Or it will insert new record. Finally it will write all records back.
        */
        void write(const std::string & name, int score);
        void clear();
        /**
         * @brief Only way how to go back to main menu is by pressing enter, escape or q.
        */
        void getInput();
        
    private:
        /**
         * @brief This will put together address to scoreboard of the level defined in @param fileName based on gameMode.
        */
        void determineName(std::string & fileName);
        /**
         * @brief Prints header
        */
        void startWin();
        /**
         * @brief It will load every record from file and displays it.
        */
        void loadSB();
        
        std::shared_ptr<WINDOW> boardWin;
        OneStringWin name;
        int gameMode;
        std::string levelName;
        std::string humanVsHumanDir;
        std::string humanVsPCDir;
};