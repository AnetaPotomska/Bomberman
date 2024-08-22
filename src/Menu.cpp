#include "Menu.h"

#include <ncurses.h>
#include <string>
#include <vector>
#include <memory> 

#include "OneStringWin.h"

Menu::Menu() {
    getmaxyx(stdscr, yMax, xMax);
    startRow = (yMax / 2) - (HEIGHT_MENU_WIN / 2);
    startCol = (xMax / 2) - (WIDTH_MENU_WIN / 2);
    menuWin = std::shared_ptr<WINDOW>(newwin(HEIGHT_MENU_WIN, WIDTH_MENU_WIN, startRow, startCol), [](WINDOW * ptr) {delwin(ptr);}); 
    name.create(WIDTH_MENU_WIN, startRow - 1, startCol);
}

void Menu::startWin() {
    wclear(menuWin.get());
    box(menuWin.get(), 0, 0);
    refresh();
    wrefresh(menuWin.get()); 
}

int Menu::show(const std::string & dialog, const std::vector<std::string> & choices) {
    keypad(menuWin.get(), true);
    int numOfChoices = choices.size();
    int choice;
    int highlight = 0;
    bool flagContinue = true;
    while(flagContinue) {
        startWin();
        mvwprintw(menuWin.get(), 1, 1, "%s", dialog.c_str());
        for(int i = 0; i < numOfChoices; i++) {
            if(i == highlight) {
                wattron(menuWin.get(), A_REVERSE);
            }
            mvwprintw(menuWin.get(), i + 3, 1, "%s", choices[i].c_str());
            wattroff(menuWin.get(), A_REVERSE);
        }
        choice = wgetch(menuWin.get());
        switch(choice) {
            case KEY_UP:
                highlight--;
                if(highlight == -1) {
                    highlight++;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if(highlight == numOfChoices) {
                    highlight--;
                }
                break;
            case 10: // ENTER
            case ' ':
                return highlight;
            case 27: // ESC
            case 'q':
                return -1;
            default:
                break;
        }
    }
    return -1;
}

void Menu::clear() {
    name.clear();
    wclear(menuWin.get());
    refresh();
    wrefresh(menuWin.get()); 
}