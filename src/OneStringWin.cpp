#include "OneStringWin.h"

#include <ncurses.h>
#include <memory>
#include <string>

void OneStringWin::create(int width, int y, int x) {
    if(width < MIN_WIDTH) {
        width = MIN_WIDTH;
    }
    nameWin = std::shared_ptr<WINDOW>(newwin(1, width, y, x), [](WINDOW * ptr) {delwin(ptr);}); 
}

void OneStringWin::printName(const std::string & name) {
    mvwprintw(nameWin.get(), 0, 0, "%s", name.c_str());
    refreshWin();
}

void OneStringWin::printName(const std::string & name1, const std::string & name2) {
    mvwprintw(nameWin.get(), 0, 0, "%s: %s", name1.c_str(), name2.c_str());
    refreshWin();
}

void OneStringWin::clear() {
    wclear(nameWin.get());
    refreshWin();
}

void OneStringWin::refreshWin() {
    refresh();
    wrefresh(nameWin.get());
}