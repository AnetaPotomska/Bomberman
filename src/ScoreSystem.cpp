#include "ScoreSystem.h"

#include "../libs/json.hpp"

ScoreSystem::ScoreSystem(const nlohmann::json & scoreSystem) {
    parseScoreSystemFromJson(scoreSystem);
}

void ScoreSystem::parseScoreSystemFromJson(const nlohmann::json & scoreSystem) {
    if(scoreSystem.contains("killing_other_player")) {
        killingOtherPlayer = scoreSystem["killing_other_player"].get<int>();
    }
    if(scoreSystem.contains("activating_another_bomb")) {
        activatingAnotherBomb = scoreSystem["activating_another_bomb"].get<int>();
    }
    if(scoreSystem.contains("picking_up_bonus")) {
        pickingUpBonus = scoreSystem["picking_up_bonus"].get<int>();
    }
    if(scoreSystem.contains("destroying_wall")) {
        destroyingWall = scoreSystem["destroying_wall"].get<int>();
    }
    if(scoreSystem.contains("placing_bomb")) {
        placingBomb = scoreSystem["placing_bomb"].get<int>();
    } 
    if(scoreSystem.contains("killing_self")) {
        killingSelf = scoreSystem["killing_self"].get<int>();
    }     
}