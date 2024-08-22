#include "InfoTable.h"

#include <ncurses.h>
#include <memory>
#include <map>
#include <string>

InfoTable::InfoTable() {
    data["Score"] = 0;
    data["Lives"] = 0;
    data["Bombs"] = 0;
    data["Flames"] = 0;
    data["Detonator"] = 0;
    data["Speed"] = 0;
    data["Bomb pass"] = 0;
}

void InfoTable::initWin(int playerId) {
    this->playerId = playerId;
    int yMax = 0;
    int xMax = 0;
    getmaxyx(stdscr, yMax, xMax);
    int yPos;
    int xPos;
    switch(playerId) {
        case 0:
            yPos = 0;
            xPos = 0;
            break;
        case 1:
            yPos = 0;
            xPos = xMax - WIDTH_INFO_WIN;
            break;
        case 2:
            yPos = yMax - HEIGHT_INFO_WIN;
            xPos = 0;
            break;
        case 3:
            yPos = yMax - HEIGHT_INFO_WIN;
            xPos = xMax - WIDTH_INFO_WIN;
            break;
        default:
            throw("More than 4 players.");
    }
    infoWin = std::shared_ptr<WINDOW>(newwin(HEIGHT_INFO_WIN, WIDTH_INFO_WIN, yPos, xPos), [](WINDOW * ptr) {delwin(ptr);}); 
    printInit();
}

void InfoTable::clear() {
    wclear(infoWin.get());
    refreshWin();
}

void InfoTable::refreshWin() {
    refresh();
    wrefresh(infoWin.get());
}

void InfoTable::updateEverything(int score, int lives, int bombsAvailable, int flamesPower, int hasRemoteControl, int speed, int hasBombPass) {
    data["Score"] = score;
    data["Lives"] = lives;
    data["Bombs"] = bombsAvailable;
    data["Flames"] = flamesPower;
    data["Detonator"] = hasRemoteControl;
    data["Speed"] = speed;
    data["Bomb pass"] = hasBombPass;
}

void InfoTable::printData() {
    // from third begins data
    int row = 3;
    for(auto & x : data) {
        mvwprintw(infoWin.get(), row, 1, "%s: %d", x.first.c_str(), x.second);
        row++;
    }
}

void InfoTable::printInit() {
    box(infoWin.get(), 0, 0);
    // first row is for headline
    mvwprintw(infoWin.get(), 1, 1, "Player %d", playerId);
    // second row is for separator
    for(int i = 0; i < WIDTH_INFO_WIN; i++) {
        mvwprintw(infoWin.get(), 2, i, "-");
    }
    printData();
    // and refresh
    refreshWin();
}