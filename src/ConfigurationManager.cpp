#include "ConfigurationManager.h"

#include <fstream>
#include <ncurses.h>
#include <string>
#include <exception>

#include "../libs/json.hpp"

void ConfigurationManager::setupConfig(const char * configPath) {
    std::ifstream configFile(configPath);
    if (!configFile) {
        throw("Cannot load config file.");
    }
    if(!(configFile >> config)) {
        throw("Cannot read from config file.");
    }
}

nlohmann::json ConfigurationManager::getPlayersKeys(int id) {
    std::string playerKey = "player_" + std::to_string(id);
    auto ret = getJSON(playerKey);
    if(!ret.contains("keys")) {
        throw ("Player doesn't have setup keys in config file.");
    }
    return ret["keys"];
}

std::string ConfigurationManager::getString(const std::string & str) {
    if (!config.contains(str)) {
        throw std::logic_error("Cannot find string in config file.");
    }
    return config[str].get<std::string>();
}


nlohmann::json ConfigurationManager::getJSON(const std::string & str)  {
    if (!config.contains(str)) {
        throw ("Cannot find string in config file.");
    }
    return config[str];
}