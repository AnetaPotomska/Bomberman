#pragma once

#include <ncurses.h>
#include <string>
#include <vector>
#include <memory> 

#include "OneStringWin.h"

#define HEIGHT_MENU_WIN 15
#define WIDTH_MENU_WIN HEIGHT_MENU_WIN * 3

/**
 * @brief Prints menu and allows user to chose where he wants to go.
*/
class Menu {
    public:
        /**
         * @brief Initilize menu window.
        */
        Menu();
        /**
         * @brief Everytime when menu is called it needs to be redo.
         * This is because ncurses is clever and updates only printable differences.
         * But it has bad side to it. It will never fully get rid of the original content.
        */
        void startWin();
        /**
         * @brief It will show all @param choices that user can choose in menu.
         * In nutshell user can move around menu using key up and down for movement, also enter for choosing and q for qiuting.
        */
        int show(const std::string & dialog, const std::vector<std::string> & choices);
        void clear();
    private:
        OneStringWin name; // shows name of the game
        std::shared_ptr<WINDOW> menuWin;
        int yMax = 0;
        int xMax = 0;
        int startRow = 0;
        int startCol = 0;
};