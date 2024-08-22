#pragma once

#include <ncurses.h>
#include <memory>
#include <string>

#define MIN_WIDTH 10

/**
 * @brief It shows single line of text in window.
*/
class OneStringWin {
    public:
        void create(int width, int y, int x);
        void clear();
        void refreshWin();
        /**
         * @brief Prints only string.
        */
        void printName(const std::string & name);
        /**
         * @brief Prints string and int in format "%s: %s".
        */
        void printName(const std::string & name1, const std::string & name2);
    private:
        std::shared_ptr<WINDOW> nameWin;
};