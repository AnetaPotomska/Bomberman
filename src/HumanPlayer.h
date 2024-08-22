#pragma once

#include <ncurses.h>
#include <memory>

#include "Player.h"
#include "Map.h"
#include "Board.h"
#include "Position.h"
#include "../libs/json.hpp"

/**
 * @brief Every human player has defined his control keys.
*/
class HumanPlayer : public Player {
    public:
        HumanPlayer() = default;
        HumanPlayer(const Position & pos, int id, std::shared_ptr<Map> map, std::shared_ptr<Board> board, const nlohmann::json & playersStats, const nlohmann::json & keysJson);
        /**
         * @brief Player has his turn limited by time.
         * In this limit player needs to decides on his turn.
        */
        virtual void doTurn() override;
    private:
        /**
         * @brief It will get keys from @param keyJson .
        */
        void parseKeysFromJson(const nlohmann::json & keysJson);
        chtype getInput();
        /**
         * @brief Based on @param input player will do move, place bomb or use remote.
        */
        bool proccessInput(chtype input);
        /**
         * @brief Player has some time for putting his turn. 
        */
        void setTimeout(int timeout);
        chtype keyUp;
        chtype keyDown;
        chtype keyLeft;
        chtype keyRight;
        chtype keyPlaceBomb;
        chtype keyRemoteControlExplodeBomb;
};