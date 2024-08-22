#pragma once

#include <ncurses.h>
#include <memory>

#include "Object.h"
#include "Map.h"
#include "Board.h"
#include "Position.h"
#include "Statistics.h"
#include "../libs/json.hpp"

/**
 * @brief Player is represented by his id.
*/
class Player : public Object {
    public:
        Player() = default;
        Player(const Position & pos, int id, std::shared_ptr<Map> map, std::shared_ptr<Board> board, const nlohmann::json & playersStats);
        int getId() const;
        int getScore() const;
        Statistics & getStats();
        bool isAlive() const;
        void updateLives(int add);
        void updateBombs(int add);
        void updateScore(int add);
        virtual void doTurn() = 0;
        /**
         * @brief If player is alive it will print him on board.
        */
        void show();
        void refreshInfoTable();
        void clearInfoTable();
    protected:
        /**
         * @brief Make move in direction given by direction. 
         * @return true if move could have been made (players didn't go to wall or bomb if he can't)
         * @return false otherwise
        */
        bool move(int direction);
        /**
         * @brief If palyer has enough bombs he can place bomb underneath him. 
        */
        bool placeBomb();
        /**
         * @brief If player has remote control he can detonate his bombs.
        */
        bool remotelyExplodeBombs();

        int id;
        std::shared_ptr<Map> map;
        std::shared_ptr<Board> board;
        Statistics stats; // holds every stats of player and infotable
};