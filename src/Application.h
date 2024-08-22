#pragma once

#include <vector>
#include <string>

#include "LevelLoader.h"
#include "ConfigurationManager.h"

#define HEIGHT_MIN_WIN 20
#define WIDTH_MIN_WIN 80

/**
 * @brief A brain that starts things according to the menu selection.
*/
class Application {
    public:
        Application(const ConfigurationManager & conf);
        /**
         * @brief Initilize ncurses and starts navigation.
        */
        void run();
    private:
        /**
         * @brief Terminal cannot be smaller than defines defined up here.
        */
        void checkTerminalWindow() const;
        /**
         * @brief Depending on what was the final choice in the menu, the given process will be started.
         * It can either play a game or show scoreboards.
         * After the proccess is done it will automatically show menu again.
        */
        void navigation();
        
        std::vector<std::string> mainMenuChoices = {"Human vs. Human", "Human vs. PC", "Scoreboard H vs. H", "Scoreboard H vs. PC", "Quit"}; // main menu choices
        ConfigurationManager conf;
        LevelLoader levels;
};