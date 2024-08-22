#include "ScoreBoardManager.h"

#include <exception>
#include <string>
#include <memory>
#include <ncurses.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "GameMode.h"
#include "OneStringWin.h"
#include "ScoreBoard.h"
#include "../libs/json.hpp"

ScoreBoardManager::ScoreBoardManager(int gameMode, const std::string & levelName, const nlohmann::json & scoreboardsDirs) :
    gameMode(gameMode),
    levelName(levelName) {
        if(!scoreboardsDirs.contains("human_vs_human_dir")) {
            throw("Config file doesn't include human vs human dir.");
        }
        if(!scoreboardsDirs.contains("human_vs_pc_dir")) {
            throw("Config file doesn't include human vs pc dir.");
        }
        humanVsHumanDir = scoreboardsDirs["human_vs_human_dir"].get<std::string>();
        humanVsPCDir = scoreboardsDirs["human_vs_pc_dir"].get<std::string>();
    }

void ScoreBoardManager::print() {
    int yMax;
    int xMax;
    getmaxyx(stdscr, yMax, xMax);
    int startRow = (yMax / 2) - (HEIGHT_SB_WIN / 2);
    int startCol = (xMax / 2) - (WIDTH_SB_WIN / 2);
    boardWin = std::shared_ptr<WINDOW>(newwin(HEIGHT_SB_WIN, WIDTH_SB_WIN, startRow, startCol), [](WINDOW * ptr) {delwin(ptr);}); 
    name.create(WIDTH_SB_WIN, startRow - 1, startCol);
    startWin();
    loadSB();
}

void ScoreBoardManager::write(const std::string & name, int score) {
    std::vector<ScoreBoard> sb;
    std::string fileName = "";
    determineName(fileName);

    // read all scoreboard
    std::ifstream input(fileName, std::ios::binary);
    if(!input) {
        input.close();
        std::ofstream newFile(fileName, std::ios::binary); // if there wasn't any scoreboard create one
        if(!newFile) {
            throw("Couldn't create file to read in scoreboard.");
        }
        newFile.close();
    }
    else { // load every record to memory
        std::string line;
        while (getline(input, line)) {
            ScoreBoard record;
            std::stringstream ss(line);
            if(!ss) {
                throw("Couldn't create string stream in scoreboard.");
            }
            if(!(ss >> record.name >> record.score)) {
                throw("Couldn't read from file in scoreboard.");
            }
            sb.push_back(record);
        }
        if (!input.eof() || input.bad()) {
            throw("Error in reading in scoreboard.");
        }
        input.close();
    }
    
    // if there was record with same name and we have bigger score remove old record
    for(auto it = sb.begin(); it != sb.end(); it++) {
        if(it->name == name) {
            if(score <= it->score) {
                return;
            } 
            it = sb.erase(it);
            break;
        }
    }

    // sort by score and then by name
    bool written = false;
    for(auto it = sb.begin(); it != sb.end(); it++) {
        if(score > it->score) {
            sb.insert(it, {name, score});
            written = true;
            break;
        }
        else if(score == it->score && name < it->name) {
            sb.insert(it, {name, score});
            written = true;
            break;
        }
    }

    // wasn't still put so do it now
    if(!written) {
        sb.push_back({name, score});
    }
    
    // write it back
    std::ofstream output(fileName, std::ios::binary);
    if(!output) {
        throw("Couldn't load file to write in scoreboard.");
    }
    for (size_t i = 0; i < sb.size(); i++) {
        // scoreboard is quite small so we will show only first 20 records (others will be forgotten)
        if(i == MAX_RECORDS) {
            break;
        }
        if(!(output << sb[i].name << " " << sb[i].score)) {
            throw("Couldn't write to file in scoreboard.");
        }
        if(i != sb.size() - 1) {
            if(!(output << std::endl)) {
                throw("Couldn't write to file in scoreboard.");
            }
        }
    }
    if (output.bad()) {
        throw("Error in writing to file in scoreboard.");
    }
    output.close();
}


void ScoreBoardManager::clear() {
    name.clear();
    wclear(boardWin.get());
    refresh();
    wrefresh(boardWin.get()); 
}

void ScoreBoardManager::getInput() {
    while(true) {
        auto choice = wgetch(boardWin.get());
        switch(choice) {
            case 10: // ENTER
            case 27: // ESC
            case 'q':
                return;
        }
    }
}

void ScoreBoardManager::determineName(std::string & fileName) {
    if(gameMode == HUMAN_VS_HUMAN) {
        fileName += humanVsHumanDir;
    }
    else {
        fileName += humanVsPCDir;
    }
    fileName += levelName;
}

void ScoreBoardManager::startWin() {
    wclear(boardWin.get());
    box(boardWin.get(), 0, 0);
    // first row is for headline
    mvwprintw(boardWin.get(), 1, 1, "Name");
    mvwprintw(boardWin.get(), 1, WIDTH_SB_WIN / 2, "Score");
    // second row is for separator
    for(int i = 0; i < WIDTH_SB_WIN; i++) {
        mvwprintw(boardWin.get(), 2, i, "-");
    }
    refresh();
    wrefresh(boardWin.get()); 
}

void ScoreBoardManager::loadSB() {
    std::string fileName = "";
    determineName(fileName);
    std::string line;
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::ofstream createFile(fileName, std::ios::binary);
        if(!createFile) {
            throw("Cannot open file.");
        }
        return;
    }
    // from third row it is for data
    int linePos = 3;
    while (std::getline(file, line)) {
        std::string name;
        std::string score;
        auto pos = line.find(" ");
        name = line.substr(0, pos);  // the part till the space
        score = line.substr(pos + 1); // the part after the space
        mvwprintw(boardWin.get(), linePos, 1, "%s", name.c_str());
        mvwprintw(boardWin.get(), linePos, WIDTH_SB_WIN / 2, "%s", score.c_str());
        refresh();
        wrefresh(boardWin.get()); 
        linePos++;
    }
    if (!file.eof() || file.bad()) {
        throw("Error in reading in scoreboard.");
    }
    file.close();
}