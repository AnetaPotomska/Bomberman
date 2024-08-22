#pragma once

#include <ncurses.h>
#include <string>

#include "../libs/json.hpp"

/**
 * @brief Handles configuration of app with config file.
 * There are about 6 different types of configs:
    * directories for scorebaord
    * directory for levels
    * players statistics
    * player 0-4 controls
    * probability of bonuses
    * score system
*/
class ConfigurationManager {
    public:
        /**
         * @brief Loads whole config defined by @param configPath to config.
        */
        void setupConfig(const char * configPath);
        /**
         * @brief Get JSON of keys for player with @param id .
         * This will want human players.
        */
        nlohmann::json getPlayersKeys(int id);
        /**
         * @brief Get string as value for @param str.
         * This uses only LevelLoader for directory where are levels.
        */
        std::string getString(const std::string & str);
        /**
         * @brief Get JSON part from config defined by @param str.
        */
        nlohmann::json getJSON(const std::string & str);
    private:
        nlohmann::json config;
};