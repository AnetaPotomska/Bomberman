#include "Board.h"

#include <string>
#include <memory>
#include <ncurses.h>

#include "Position.h"

void Board::initilize(int numOfRows, int numOfCols) {
    this->numOfRows = numOfRows;
    this->numOfCols = numOfCols;
    getmaxyx(stdscr, yMax, xMax);
    startRow = (yMax / 2) - (numOfRows / 2);
    startCol = (xMax / 2) - (numOfCols / 2);
    boardWin = std::shared_ptr<WINDOW>(newwin(numOfRows, numOfCols, startRow, startCol), [](WINDOW * ptr) {delwin(ptr);}); 
    name.create(numOfCols, startRow - 1, startCol);
    turn.create(numOfCols, startRow + numOfRows, startCol);
    name.printName("BOMBERMAN");
    refreshWin();
}

void Board::draw(const Position & pos, char symbol) {
    mvwaddch(boardWin.get(), pos.y, pos.x, symbol);
}

void Board::clear() {
    name.clear();
    turn.clear();
    clearBoard();
    refreshWin();
}

void Board::refreshWin() {
    refresh();
    wrefresh(boardWin.get()); 
}


void Board::printWhoseIsTurn(const std::string & who) {
    turn.printName("ON TURN", who);
}

std::shared_ptr<WINDOW> Board::getBoardWin() const {
    return boardWin;
}

int Board::getHeight() const {
    return numOfRows;
}

int Board::getWidth() const {
    return numOfCols;
}

void Board::clearBoard() {
    wclear(boardWin.get());
}