#include "Map.h"

#include <memory>
#include <queue>
#include <map>
#include <utility>
#include <vector>
#include <string>
#include <exception>
#include <cstdlib>

#include "../libs/json.hpp"
#include "Board.h"
#include "ScoreSystem.h"
#include "Bomb.h"
#include "Wall.h"
#include "Position.h"
#include "Empty.h"
#include "Booster.h"
#include "Flame.h"
#include "AddBomb.h"
#include "AddFlames.h"
#include "AddLife.h"
#include "AddRemoteControl.h"
#include "AddSpeed.h"
#include "AddBombPass.h"

Map::Map(std::shared_ptr<Board> board, const nlohmann::json & boostersProbabilites, const ScoreSystem & scoreSystem) :
    board(board),
    scoreSystem(scoreSystem) {
        parseProbabilitiesFromJson(boostersProbabilites);
        srand((unsigned int)time(NULL));
    }

std::queue<Bomb> Map::searchForBombsToExplode() {
    std::queue<Bomb> bombsAboutToExplode;
    for (auto it = bombs.begin(); it != bombs.end(); ) {
        if(it->second.isGoingToExplode()) {
            bombsAboutToExplode.push(it->second);
            bombs.erase(it++);
        }
        else {
            it++;
        }
    }
    return bombsAboutToExplode;
}

bool Map::isAnyBombAboutToExplode() {
    for(auto & x : bombs) {
        if(x.second.isGoingToExplode()) {
            return true;
        }
    }
    return false;
}

// id, score to add , bombs to add
std::map<int, std::pair<int, int>> Map::detonateAll(int playerCnt) {
    std::map<int, std::pair<int, int>> ret;
    for(int i = 0; i < playerCnt; i++) {
        ret[i] = {0, 0};
    }
    auto bombsAboutToExplode = searchForBombsToExplode();
    while(!bombsAboutToExplode.empty()) {
        auto bomb = bombsAboutToExplode.front();
        bombsAboutToExplode.pop();
        ret[bomb.getId()].second++;
        ret[bomb.getId()].first += addFlames(bomb.explode(walls)) + scoreSystem.placingBomb;
    }
    return ret;
}

bool Map::isCollidingWithFlame(const Position & pos) {
    return flames.count(pos);
}

bool Map::isCollidingWithBonus(const Position & pos) {
    return boosters.count(pos);
}

bool Map::isCollidingWithWall(const Position & pos) {
    return walls.count(pos);
}

bool Map::isCollidingWithBomb(const Position & pos) {
    return bombs.count(pos);
}

void Map::decreaseCountdownOfBombs() {
    for(auto & x : bombs) {
        x.second.updateCountdown(-1);
    }
}

void Map::removePlayersPosition(int id) {
    if(playersPositions.count(id)) {
        playersPositions.erase(id);
    }
}

int Map::detonateById(int id) {
    int scoreToAdd = 0;
    std::vector<Position> whereErase;
    for(auto & x : bombs) {
        if(x.second.getId() == id) {
            whereErase.push_back(x.first);
        }
    }
    for(auto & x : whereErase) {
        Bomb tmpBomb = bombs[x];
        bombs.erase(x);
        scoreToAdd += addFlames(tmpBomb.explode(walls)) + scoreSystem.placingBomb;
    }
    return scoreToAdd;
}

void Map::show() {
    for(auto & x : emptySpaces) {
        board->draw(x.first, x.second.getIcon());
    }
    for(auto & x : walls) {
        board->draw(x.first, x.second.getIcon());
    }
    for(auto & x : boosters) {
        board->draw(x.first, x.second->getIcon());
    }
    for(auto & x : bombs) {
        board->draw(x.first, x.second.getIcon());
    }
    for(auto & x : flames) {
        board->draw(x.first, x.second.getIcon());
    }
}

bool Map::doesBoxExists(const Position & pos) {
    return walls.count(pos);
}

bool Map::isOnOwnBomb(const Position & pos, int id) {
    return isCollidingWithBomb(pos) && bombs[pos].getId() == id;
}

bool Map::destroyAllFlames() {
    bool ret = false;
    for(auto & x : flames) {
        addEmpty(Empty(x.first));
        ret = true;
    }
    flames.clear();
    return ret;
}

void Map::addWall(const Wall & wall) {
    walls[wall.getPos()] = wall;
}

void Map::addBonus(const Position & pos) {
    if(totalProbability > 0) {
        int num = 1 + (rand() % totalProbability);
        int cntProbability = 1;
        for(auto & x : probabilities) {
            if(num > cntProbability && num < cntProbability + x.first) {
                addBonusByCharacter(x.second, pos);
                break;
            }
            cntProbability += x.first;
        }
    }
}

void Map::addBonusByCharacter(char ch, const Position & pos) {
    switch(ch) {
        case 'F':
            boosters[pos] = std::shared_ptr<Booster>(new AddFlames(pos));
            break;
        case 'S':
            boosters[pos] = std::shared_ptr<Booster>(new AddSpeed(pos));
            break;
        case 'R':
            boosters[pos] = std::shared_ptr<Booster>(new AddRemoteControl(pos));
            break;
        case 'L':
            boosters[pos] = std::shared_ptr<Booster>(new AddLife(pos));
            break;
        case 'B':
            boosters[pos] = std::shared_ptr<Booster>(new AddBomb(pos));
            break;
        case 'P':
            boosters[pos] = std::shared_ptr<Booster>(new AddBombPass(pos));
            break;
        default:
            throw("Wrong bonus character.");    
    }
}

void Map::addPlayersPosition(const Position & pos, int id) {
    playersPositions[id] = pos;
}

void Map::addBomb(const Bomb & bomb) {
    bombs[bomb.getPos()] = bomb;
}

void Map::addEmpty(const Empty & empty) {
    emptySpaces[empty.getPos()] = empty;
}

void Map::addWallOnBorder() {
    int height = board->getHeight();
    int width = board->getWidth();
    // up wall
    for(int i = 0; i < width; i++) {
        addWall(Wall(Position(0, i), '#', false));
    }
    // left wall
    for(int i = 0; i < height; i++) {
        addWall(Wall(Position(i, 0), '#', false));
    }
    // down wall
    for(int i = 0; i < width; i++) {
        addWall(Wall(Position(height - 1, i), '#', false));
    }
    // right wall
    for(int i = 0; i < height; i++) {
        addWall(Wall(Position(i, width - 1), '#', false));
    }
}

bool Map::isPlayerOnMap(int id) const {
    return playersPositions.count(id);
}

bool Map::isAbleToMove(const Position & pos) {
    return !isCollidingWithWall(pos) && !isCollidingWithBomb(pos);
}

int Map::getIdOfFlame(const Position & pos) {
    return flames[pos].getId();
}

std::vector<Bomb> Map::findBombById(int id) {
    std::vector<Bomb> bombsToRet;
    for(auto it = bombs.begin(); it != bombs.end(); it++) {
        if(it->second.getId() == id) {
            bombsToRet.push_back(it->second);
        }
    }
    return bombsToRet;
}

std::vector<Bomb> Map::getBombs() const {
    std::vector<Bomb> bombsToRet;
    for(auto & x : bombs) {
        bombsToRet.push_back(x.second);
    }
    return bombsToRet;
}

std::vector<Position> Map::getDestroyableWallsPositions() {
    std::vector<Position> wallsToRet; 
    for(auto & x : walls) {
        if(x.second.isDestroyable()) {
            wallsToRet.push_back(x.first);
        }
    }
    return wallsToRet;
}

std::map<Position, Empty> Map::getEmptySpaces() const {
    return emptySpaces;
}

std::map<Position, Wall> Map::getWalls() const {
    return walls;
}

std::shared_ptr<Booster> Map::getBonus(const Position & pos) {
    auto ret = boosters.at(pos);
    boosters.erase(pos);
    return ret;
}

std::vector<Position> Map::getPlayersPositionsExceptSelf(int id) const {
    std::vector<Position> toRet;
    for(auto & x : playersPositions) {
        if(x.first != id)
            toRet.push_back(x.second);
    }
    return toRet;
}

std::vector<Position> Map::getBoostersPositions() const {
    std::vector<Position> toRet;
    for(auto & x : boosters) {
        toRet.push_back(x.first);
    }
    return toRet;
}

void Map::addToProbabilities(const nlohmann::json & playersStats, const std::string & name, char ch) {
    if(playersStats.contains(name)) {
        if(playersStats[name].get<int>() > 0) {
            probabilities.push_back({playersStats[name].get<int>(), ch});
            totalProbability += playersStats[name].get<int>();
        }
    }
    else {
        probabilities.push_back({20, ch});
        totalProbability += 20;
    }
}

void Map::parseProbabilitiesFromJson(const nlohmann::json & playersStats) { 
    addToProbabilities(playersStats, "add_flames", 'F');
    addToProbabilities(playersStats, "add_speed", 'S');
    addToProbabilities(playersStats, "add_remote_control", 'R');
    addToProbabilities(playersStats, "add_lifes", 'L');
    addToProbabilities(playersStats, "add_bombs", 'B');
    addToProbabilities(playersStats, "add_bombs_pass", 'P');
}

int Map::addFlames(const std::map<Position, Flame> & newFlames) {
    int scoreToAdd = 0;
    for(auto & x : newFlames) {
        if(isCollidingWithWall(x.second.getPos())) {
            if(walls[x.second.getPos()].isDestroyable()) {
                walls.erase(x.second.getPos());
                addBonus(x.first);
                addFlame(x.second);
                scoreToAdd += scoreSystem.destroyingWall;
            }
        }
        else if(isCollidingWithBomb(x.second.getPos())) {
            Bomb tmp = bombs[x.second.getPos()];
            bombs.erase(x.second.getPos());
            addFlames(tmp.explode(walls));
            
            scoreToAdd += scoreSystem.activatingAnotherBomb;
        }
        else if(isCollidingWithBonus(x.second.getPos())) {
            boosters.erase(x.second.getPos());
            addFlame(x.second);
        }
        else if(!isCollidingWithFlame(x.second.getPos())) {
            addFlame(x.second);
        }
    }
    return scoreToAdd;
}

void Map::addFlame(const Flame & flame) {
    flames[flame.getPos()] = flame;
}