#include "Statistics.h"

#include <exception>

#include "InfoTable.h"
#include "../libs/json.hpp"

Statistics::Statistics(int id, const nlohmann::json & playersStats) :
    id(id) {
        parseStatsFromJson(playersStats);
        infoTable.updateEverything(score, lives, bombsAvailable, flamesPower, hasRemoteControl, speed, hasBombPass);
        infoTable.initWin(id);
    }

void Statistics::updateLives(int add) {
    lives += add;
}

void Statistics::updateBombs(int add) {
    bombsAvailable += add;
    bombsPlaced -= add;
}

void Statistics::updateScore(int add) {
    score += add;
}

void Statistics::updateFlames(int add) {
    flamesPower += add;
}

void Statistics::updateSpeed(int add) {
    if(speed + add > 0) {
        speed += add;
    }
}

void Statistics::addBomb(int add) {
    bombsAvailable += add;
}

void Statistics::addRemoteControl() {
    hasRemoteControl = true;
}

void Statistics::addBombPass() {
    hasBombPass = true;
}


bool Statistics::isAlive() const {
    return lives > 0;
}

void Statistics::refreshInfoTable() {
    infoTable.updateEverything(score, lives, bombsAvailable, flamesPower, hasRemoteControl, speed, hasBombPass);
    infoTable.printData();
    infoTable.refreshWin();
}

void Statistics::clearInfoTable() {
    infoTable.clear();
}

void Statistics::parseStatsFromJson(const nlohmann::json & playersStats) {
    if(playersStats.contains("speed")) {
        speed = playersStats["speed"].get<int>();
    }
    if(playersStats.contains("lives")) {
        lives = playersStats["lives"].get<int>();
    }
    if(lives < 1) {
        throw("Players cannot have 0 or negative lives in begining.");
    }
    if(playersStats.contains("bombs_available")) {
        bombsAvailable = playersStats["bombs_available"].get<int>();
    }
    if(playersStats.contains("flames_power")) {
        flamesPower = playersStats["flames_power"].get<int>();
    }
    if(playersStats.contains("counter_on_bombs")) {
        counterOnBombs = playersStats["counter_on_bombs"].get<int>();
    }
    if(playersStats.contains("has_remote_control")) {
        hasRemoteControl = playersStats["has_remote_control"].get<int>();
    }
    if(playersStats.contains("has_bomb_pass")) {
        hasBombPass = playersStats["has_bomb_pass"].get<int>();
    }
}