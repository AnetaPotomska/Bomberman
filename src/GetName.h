#pragma once

#include <ncurses.h>
#include <memory>
#include <string>

#include "Player.h"

#define HEIGHT_NAME_WIN 5
#define WIDTH_NAME_WIN 32

/**
 * @brief It will draw window asking for players name. Which it will later return.
*/
class GetName {
    public:
        /**
         * @brief Initilize window and call printInit.
        */
        GetName(std::shared_ptr<Player> player);
        void clear();
        /**
         * @brief It will let user type and modify his name.
         * When user is ready he can press enter and it will return his name.
         * Name cannot be longer than 14 characters, also it can't contain unprintable characters or whitespaces.
        */
        std::string getName() const;
    private:
        /**
         * @brief Prints instructions in window.
        */
        void printInit();
        std::shared_ptr<WINDOW> nameWin;
        std::shared_ptr<Player> player;
};