#include "GetName.h"

#include <ncurses.h>
#include <memory>
#include <string>

#include "Player.h"

GetName::GetName(std::shared_ptr<Player> player) {
    this->player = player;
    int yMax = 0;
    int xMax = 0;
    getmaxyx(stdscr, yMax, xMax);
    int startRow = (yMax / 2) - (HEIGHT_NAME_WIN / 2);
    int startCol = (xMax / 2) - (WIDTH_NAME_WIN / 2);
    nameWin = std::shared_ptr<WINDOW>(newwin(HEIGHT_NAME_WIN, WIDTH_NAME_WIN, startRow, startCol), [](WINDOW * ptr) {delwin(ptr);}); 
    printInit();
}

void GetName::clear() {
    wclear(nameWin.get());
    refresh();
    wrefresh(nameWin.get()); 
}

std::string GetName::getName() const {
    std::string name;
    chtype ch;
    int x = 1;
    // third row is for name of player
    int y = 3;
    while (true) {
        ch = wgetch(nameWin.get());
        if (ch == 10 && name.size() > 0) { // ENTER
            return name;
        } 
        else if (ch == KEY_BACKSPACE || ch == 127) {
            if (name.size() > 0) {
                name.erase(name.size() - 1);
                mvwprintw(nameWin.get(), y, x - 1, " ");
                x--;
            }
        } 
        else if (name.size() < 14 && isprint(ch) && !isspace(ch)) {
            name.push_back(ch);
            mvwprintw(nameWin.get(), y, x, "%c", ch);
            x++;
        }
        wrefresh(nameWin.get());
    }
}


void GetName::printInit() {
    box(nameWin.get(), 0, 0);
    // first row is for headline
    mvwprintw(nameWin.get(), 1, 1, "Player %d name: (max 14 ch.)", player->getId());
    // second row is for separator 
    for(int i = 0; i < WIDTH_NAME_WIN; i++) {
        mvwprintw(nameWin.get(), 2, i, "-");
    }
    // and refresh
    refresh();
    wrefresh(nameWin.get());
}