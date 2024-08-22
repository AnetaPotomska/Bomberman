#include "LevelLoader.h"

#include <vector>
#include <string>
#include <filesystem>
#include <utility>
#include <dirent.h>
#include <string>
#include <fstream>
#include <exception>

#include "GameMode.h"
#include "PlayerType.h"
#include "Game.h"
#include "Position.h"

LevelLoader::LevelLoader(const std::string & levelsDir) :
    levelsDir(levelsDir) {}


void LevelLoader::loadLevels() {
    DIR * dir;
    struct dirent * entry;
    if ((dir = opendir(levelsDir.c_str())) != NULL) {
        while (( entry = readdir(dir)) != NULL ) {
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) { // do not add . or ..
                continue;
            }
            fileNames.push_back((std::string)entry->d_name);
        }
        closedir(dir);
    } 
    else {
        throw("Cannot open DIR");
    }
}

void LevelLoader::loadLevel(int levelChoice, Game & game, int gameMode) {
    loadLevelSize(levelChoice, game);
    std::string fileName = levelsDir + fileNames[levelChoice];
    std::string line;
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        throw("Cannot open file.");
    }
    int playerCnt = 0;
    int linePos = 0;
    while (std::getline(file, line)) {
        for(size_t i = 0; i < line.size(); i++) {
            if(line[i] == 'o') {
                // first is always human
                if(playerCnt == 0) {
                    game.addPlayer(Position(linePos + 1, i + 1), HUMAN);
                }
                // others depends on gameMode
                else if (playerCnt >= 1) {
                    if(gameMode == HUMAN_VS_HUMAN) {
                        game.addPlayer(Position(linePos + 1, i + 1), HUMAN);
                    }
                    else {
                        game.addPlayer(Position(linePos + 1, i + 1), PC);
                    }
                }
                game.addObject(Position(linePos + 1, i + 1), '_'); // player is on empty space
                playerCnt++;
            }
            else {
                game.addObject(Position(linePos + 1, i + 1), line[i]);
            }
            if(playerCnt > 4) {
                throw("Too many players on board.");
            }
        }
        linePos++;
    }
    if (!file.eof() || file.bad()) {
        throw("Error in loading in levelLoader.");
    }
    if(playerCnt < 2) {
        throw("Too little players on board.");
    }
    file.close();
}

void LevelLoader::loadLevelSize(int levelChoice, Game & game) {
    std::string fileName = levelsDir + fileNames[levelChoice];
    std::string line;
    std::ifstream file(fileName, std::ios::binary);
    int numOfRows = 0;
    int numOfCols = 0;
    if (!file.is_open()) {
        throw("Cannot open file.");
    }
    while (std::getline(file, line)) {
        numOfRows++;
        if (numOfRows == 1) {
            numOfCols = line.size();
        }
        else if(numOfCols != (int)line.size()) {
            throw("Wrong map size.");
        }
    }
    if (!file.eof() || file.bad()) {
        throw("Error in reading in levelLoader.");
    }
    file.close();
    game.initilizeBoard(numOfRows + 2, numOfCols + 2);
}