#pragma once

#include <memory>
#include <vector>

#include "PCPlayer.h"
#include "Map.h"
#include "Board.h"
#include "Position.h"
#include "../libs/json.hpp"

/**
 * @brief Box lover is the easiest AI to beat.
 * His only goal is destroy all boxes (destroable walls). 
 * When he is done destroying every single box in the game he will be sad.
 * So that means that he will not partricipate in the game anymore and he will commit suicide-
 * He is not stupid and will take cover before his own bombs. 
 * But see he cares only for his boxes and himself so he won't try to hide from ohers bombs.
 * He wants to be fast in his job so he will use detonator first chance given.
 * That also means he will try to place as many bombs he can, so boxes could be destoryed faster.
 * But before that he will always will value hiding more.
*/
class BoxLoverPC : public PCPlayer {
    public:
        BoxLoverPC(const Position & pos, int id, std::shared_ptr<Map> map, std::shared_ptr<Board> board, const nlohmann::json & playersStats);
        /**
         * @brief Main brain behind what box AI would do.
         * Most what was described up is happening in this method.
        */
        virtual void doTurn() override;
    private:
        /**
         * @brief Box will only get his own bomb and will try them explode.
         * Based on possible explosion he will for example take cover.
         * He will take bombs and won't look au their counter when he simulate the explosion.
         * This simulation is happening in PCPlayers iWillBeHit and targetWouldBeHit.
        */
        virtual std::vector<Bomb> getBombs() const override;
        bool commitSuicide = false; 
};