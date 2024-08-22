#pragma once

#include <string>
#include <memory>
#include <ncurses.h>

#include "OneStringWin.h"
#include "Position.h"

/**
 * @brief Board is in charge of rendering for game.
*/
class Board {
    public:
        /**
         * @brief Initilize main game window along with gameName and whose turn it is.
        */
        void initilize(int numOfRows, int numOfCols);
        /**
         * @brief Draw @param symbol at positon given by @param pos.
        */
        void draw(const Position & pos, char symbol);
        /**
         * @brief Clears all created windows.
        */
        void clear();
        /**
         * @brief Refresh main window.
        */
        void refreshWin();
        /**
         * This affects turn window.
        */
        void printWhoseIsTurn(const std::string & who);
        std::shared_ptr<WINDOW> getBoardWin() const;
        int getHeight() const;
        int getWidth() const;
        void clearBoard();
    private: 
        OneStringWin name; // shows name of the game
        OneStringWin turn; // show whose turn it is
        std::shared_ptr<WINDOW> boardWin;
        int yMax = 0;
        int xMax = 0;
        int startRow = 0;
        int startCol = 0;
        int numOfRows = 0;
        int numOfCols = 0;
};