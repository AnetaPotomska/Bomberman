#include "Game.h"

#include <string>
#include <memory>
#include <vector>
#include <exception>
#include <cstdio>
#include <utility>
#include <map>
#include <chrono>
#include <thread>

#include "ConfigurationManager.h"
#include "Position.h"
#include "Board.h"
#include "Map.h"
#include "Player.h"
#include "ScoreBoardManager.h"
#include "HumanPlayer.h"
#include "BoxLoverPC.h"
#include "ScoreLoverPC.h"
#include "GameMode.h"
#include "PlayerType.h"
#include "GetName.h"
#include "Empty.h"
#include "Wall.h"

Game::Game(int gameMode, const std::string & levelName, const ConfigurationManager & conf) :
    gameMode(gameMode),
    levelName(levelName),
    conf(conf),
    scoreSystem(this->conf.getJSON("score_system")) {
        board = std::shared_ptr<Board>(new Board());
        try {
            map = std::shared_ptr<Map>(new Map(board, this->conf.getJSON("probabilities_of_bonuses"), scoreSystem));
        }
        catch (const std::exception & err) {
            printf("Error: %s\n", err.what());
            board->clear();
            throw(err);
        }
        catch (const char * err) {
            printf("Error: %s\n", err);
            board->clear();
            throw(err);
        }
    }

void Game::addPlayer(const Position & pos, int playerType) {
    if(playerType == HUMAN) {
        players.push_back(std::shared_ptr<Player>(new HumanPlayer(pos, playersAlive, map, board, conf.getJSON("players_stats"), conf.getPlayersKeys(humanPlayers))));
        humanPlayers++;
    }
    else {
        if(PCPlayers == 0) {
            players.push_back(std::shared_ptr<Player>(new BoxLoverPC(pos, playersAlive, map, board, conf.getJSON("players_stats"))));
        }
        else {
            players.push_back(std::shared_ptr<Player>(new ScoreLoverPC(pos, playersAlive, map, board, conf.getJSON("players_stats"), scoreSystem)));
        }
        
        PCPlayers++;
    }
    playersAlive++;
}

void Game::addObject(const Position & pos, char symbol) {
    switch(symbol) {
        case '#':
            map->addWall(Wall(pos, symbol, false));
            break;
        case '+':
            map->addWall(Wall(pos, symbol, true));
            break;
        case '_': 
            map->addEmpty(Empty(pos));
            break;
        default:
            throw("Unkown object.");
    }
}

void Game::initilizeBoard(int numOfRows, int numOfCols) {
    board->initilize(numOfRows, numOfCols);
}

void Game::startGame() {
    try {
        map->addWallOnBorder();
        showEverything();
        startGameLoop();
    }
    catch (const std::exception & err) {
        printf("Error: %s\n", err.what());
        board->clear();
        for(auto & x : players) {
            x->clearInfoTable();
        }
        throw(err);
    }
    catch (const char * err) {
        printf("Error: %s\n", err);
        board->clear();
        for(auto & x : players) {
            x->clearInfoTable();
        }
        throw(err);
    }
    board->clear();
    for(auto & x : players) {
        x->clearInfoTable();
    }
    getName();
}

void Game::getName() {
    ScoreBoardManager sb(gameMode, levelName, conf.getJSON("scoreboards"));
    for(int i = 0; i < humanPlayers; i++) {
        GetName getName(players[i]);
        try {
            sb.write(getName.getName(), players[i]->getScore());
        }
        catch (const std::exception & err) {
            printf("Error: %s\n", err.what());
            getName.clear();
            throw(err);
        }
        catch (const char * err) {
            printf("Error: %s\n", err);
            getName.clear();
            throw(err);
        }
        getName.clear();
    }
}

void Game::startGameLoop() {
    while(isUp) {
        processInput();
		updateState();
		redraw();
    }
}

void Game::processInput() {
    if(map->destroyAllFlames()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        showEverything();
    }
    if(map->isAnyBombAboutToExplode()) {
        board->printWhoseIsTurn("B");
        // id, score to add, bombs to add
        std::map<int, std::pair<int,int>> toAdd = map->detonateAll(players.size());
        for(auto & x : toAdd) {
            players[x.first]->updateScore(x.second.first);
            players[x.first]->updateBombs(x.second.second);
        }
    }
    else { // if bombs are no exploding we are taking input from players
        if((size_t)playerOnTurn >= players.size()) {
            playerOnTurn = 0;
        }
        while(!players[playerOnTurn]->isAlive()) {
            playerOnTurn++;
        }
        board->printWhoseIsTurn(std::to_string(playerOnTurn));
        players.at(playerOnTurn)->doTurn();
        map->addPlayersPosition(players.at(playerOnTurn)->getPos(), playerOnTurn);
        playerOnTurn++;
        map->decreaseCountdownOfBombs(); // when bomb is currently detonating we are not going to decrease countdown
    }
}

void Game::updateState() {
    for(size_t i = 0; i < players.size(); i++) {
        if(map->isCollidingWithFlame(players[i]->getPos())) {
            int idOfKiller = map->getIdOfFlame(players[i]->getPos());
            if(idOfKiller != (int)i) {
                players[idOfKiller]->updateScore(scoreSystem.killingOtherPlayer);
            }
            else {
                players[idOfKiller]->updateScore(scoreSystem.killingSelf);
            }
            players[i]->updateLives(-1);
            if(!players[i]->isAlive()) {
                map->removePlayersPosition(i);
                playersAlive--;
            }
        }
    }
    if(playersAlive < 2) {
        isUp = false;
    }
    for(size_t i = 0; i < players.size(); i++) {
        if(map->isCollidingWithBonus(players[i]->getPos())) {
            map->getBonus(players[i]->getPos())->useBonus(players[i]->getStats());
            players[i]->updateScore(scoreSystem.pickingUpBonus);
        }
    }
}

void Game::redraw() {
    showEverything();
    for(auto & x : players) {
        x->refreshInfoTable();
    }
}

void Game::showEverything() {
    map->show();
    for(size_t i = 0; i < players.size(); i++) {
        players[i]->show();
    }
    board->refreshWin();
}