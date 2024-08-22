#pragma once

#include <vector>
#include <memory>

#include "PCPlayer.h"
#include "Map.h"
#include "Board.h"
#include "Position.h"
#include "ScoreSystem.h"
#include "../libs/json.hpp"

/**
 * @brief Score lover AI is medium hard AI.
 * His only goal is having the biggest score.
 * He will try to find richest spot by score and tries to get it.
 * Every object in this game has score on it which scoreLover know in scoreSystem.
 * He is aware of his surroundings so he will take cover even from others bombs.
*/
class ScoreLoverPC : public PCPlayer {
    public:
        ScoreLoverPC() = default;
        ScoreLoverPC(const Position & pos, int id, std::shared_ptr<Map> map, std::shared_ptr<Board> board, const nlohmann::json & playersStats, const ScoreSystem & scoreSystem);
        /**
         * @brief Brain behind this AI behaviour. Most was described up here.
         * One thing that this AI has to be carefull is that if his goal is bonus.
         * If it is he will not placeBombs to get it but rather just go up to it. 
        */
        virtual void doTurn() override;
    private:
        /**
         * @brief gets every bomb on map.
        */
        virtual std::vector<Bomb> getBombs() const override;
        /**
         * @brief decides where to go. 
         * AI can go only for 4 sources of score:
            * killing player
            * picking up bonus
            * placing own bomb
            * destroying box
        * So he will look what sources are possible to get and then 
        * decides where to go or do based on score system.
        */
        bool decideOnGoal();

        bool targetIsBonus = false;
        ScoreSystem scoreSystem;
};