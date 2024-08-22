#pragma once

#include <vector>
#include <string>

#include "Game.h"

/**
 * @brief Takes care of loading levels from the folder specified in the define.
 * In the future it will use ConfigurationManager to get level dir.
*/
struct LevelLoader {
    public:
        LevelLoader(const std::string & levelsDir);
        /**
         * @brief Loads all files from the levels folder and stores them in fileNames.
        */
        void loadLevels();
        /**
         * @brief Loads the specific composition of the level.
         * Ans saves it to game.
        */
        void loadLevel(int levelChoice, Game & game, int gameMode);
        /**
         * @brief Gets game board size and initilize board window with it (through game).
        */
        void loadLevelSize(int levelChoice, Game & game);
        std::vector<std::string> fileNames;
    private:
        std::string levelsDir;
};