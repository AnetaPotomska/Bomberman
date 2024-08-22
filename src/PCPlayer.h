#pragma once

#include <set>
#include <map>
#include <deque>
#include <vector>

#include "Player.h"
#include "Position.h"
#include "Empty.h"
#include "Map.h"
#include "Board.h"
#include "../libs/json.hpp"

/**
 * @brief This class represents AI player.
*/
class PCPlayer : public Player {
    public:
        PCPlayer() = default;
        PCPlayer(const Position & pos, int id, std::shared_ptr<Map> map, std::shared_ptr<Board> board, const nlohmann::json & playersStats);
    protected:
        /**
         * @brief Do BFS on @param graph , where he needs to find path from @param start to @param end and writes this path to @param parh .
         * @return true If path can be found.
        */
        bool findPath(const std::map<Position, std::set<Position>> & graph, const Position & start, const Position & end, std::deque<Position> & path);
        /**
         * @brief add to @param graph on @param posToAdd every neighbour of this pos taht is empty spot (defined in @param emptySpaces).
        */
        void addToGraph(std::map<Position, std::set<Position>> & graph, const std::map<Position, Empty> & emptySpaces, const Position & posToAdd);
        /**
         * @brief Go through every pos in @param emptySpaces , call on them addToGraph.
        */
        std::map<Position, std::set<Position>> toGraph (std::map<Position, Empty> emptySpaces);
        /**
         * @brief Based on @param path player will move.
        */
        void goTowardsGoal(std::deque<Position> & path);
        /**
         * @brief It will look at every possible target in @param targets . And will try find path to them.
         * If path could be found, we will write into @param newTarget and @param newPath and @return true.
        */
        bool targetGoal(const std::vector<Position> & targets, Position & newTarget, std::deque<Position> & newPath);
        virtual std::vector<Bomb> getBombs() const = 0;
        /**
         * @brief It will simulate explosion of bombs got from getBombs. And will ccreate path how to get to emptySpace where palyer wouldn't be hit.
        */
        void takeCover();
        /**
         * @return true if in simulation where all bombs got from getBombs would explode, and explosion would hit us. 
        */
        bool iWillBeHit();
        /**
         * @return true if in simulation where all bombs got from getBombs would explode, and explosion would hit target. 
        */
        bool targetWouldBeHit();
        bool targetWasDestroyed();

        Position target;
        bool goalIsTargeted = false;
        std::deque<Position> pathToGetToGoal;
        bool takingCover = false;
        std::deque<Position> pathToGetToCover;
};