#pragma once

#include <memory>
#include <queue>
#include <map>
#include <utility>
#include <vector>
#include <string>

#include "../libs/json.hpp"
#include "Board.h"
#include "ScoreSystem.h"
#include "Bomb.h"
#include "Wall.h"
#include "Position.h"
#include "Empty.h"
#include "Booster.h"
#include "Flame.h"

/**
 * @brief Map holds every object except of the player (excpet it holds players positions).
*/
class Map {
    public:
        Map(std::shared_ptr<Board> board, const nlohmann::json & boostersProbabilites, const ScoreSystem & scoreSystem);
        /**
         * @brief Every bomb that has its count down to 0 will be placed in queue bombsAboutToExplode and deleted from bombs.
        */
        std::queue<Bomb> searchForBombsToExplode();
        /**
         * @brief Asks bombs if there are upcomming explosions.
        */
        bool isAnyBombAboutToExplode();
        /**
         * @brief This will setup all bombs in bombsAboutToExplode to explode.
         * @return map with id of player and corresponding score and number of bombs that were exploded.
        */
        std::map<int, std::pair<int, int>> detonateAll(int playerCnt);
        bool isCollidingWithFlame(const Position & pos);
        bool isCollidingWithBonus(const Position & pos);
        bool isCollidingWithWall(const Position & pos);
        bool isCollidingWithBomb(const Position & pos);
        void decreaseCountdownOfBombs();
        void removePlayersPosition(int id);
        /**
         * @brief Every bomb with playerId == @param id will be exploded.
         * No matter of its countdown.
        */
        int detonateById(int id);
        /**
         * @brief Draw on board every object map holds.
         * Some objects can be on same position, so order really matter.
        */
        void show();
        /**
         * Box mean destroyable wall.
        */
        bool doesBoxExists(const Position & pos);
        /**
         * @return true if player is standing on his own bomb.
        */
        bool isOnOwnBomb(const Position & pos, int id);
        /**
         * @brief Clears map from all flames and adds empty spots instead.
        */
        bool destroyAllFlames();
        void addWall(const Wall & wall);
        /**
         * @brief When destroyable wall is destroyed there is some chance that it will drop bonus.
         * These chances are defined in probabilities. It will randomly create number
         * from 1 to totalProbability and look in probabilities where it lands.
        */
        void addBonus(const Position & pos);
        /**
         * @brief Creates Bonus on @param pos based on @param ch.
        */
        void addBonusByCharacter(char ch, const Position & pos);
        void addPlayersPosition(const Position & pos, int id);
        void addBomb(const Bomb & bomb);
        void addEmpty(const Empty & empty);
        /**
         * @brief Because levels doesn't need to include border this method will add it.
        */
        void addWallOnBorder();
        /**
         * @return true if there is record for players id in map playersPositions.
        */
        bool isPlayerOnMap(int id) const;
        /**
         * @brief It will determine if player on position @param pos is in bomb or wall.
        */
        bool isAbleToMove(const Position & pos);
        int getIdOfFlame(const Position & pos);
        /**
         * @return every Bomb with @param id.
        */
        std::vector<Bomb> findBombById(int id);
        /**
         * @brief return every Bomb.
        */
        std::vector<Bomb> getBombs() const;
        std::vector<Position> getDestroyableWallsPositions();
        std::map<Position, Empty> getEmptySpaces() const;
        std::map<Position, Wall> getWalls() const;
        std::shared_ptr<Booster> getBonus(const Position & pos);
        std::vector<Position> getPlayersPositionsExceptSelf(int id) const;
        std::vector<Position> getBoostersPositions() const;
    private:
        /**
         * @brief It will look into @param playersStats and finds there @param name.
         * Found thing is number of how probable is that bonus of @param ch will be spawned.
         * If there will not be found any record it will assume probability of 20.
        */
        void addToProbabilities(const nlohmann::json & playersStats, const std::string & name, char ch);
        /**
         * @brief Calls addToProbabilities on every possible bonus.
        */
        void parseProbabilitiesFromJson(const nlohmann::json & playersStats);
        /**
         * @brief Generally it will add flames to flames.
         * But it also has some collision logic.
         * If flame is colliding with wall and this wall is destroyable it can drop bonus.
         * If flame is colliding with bomb it will activate it (no matter of it countdown).
         * If flame is colliding with bonus it will destroy it.
         * @return score to add.
        */
        int addFlames(const std::map<Position, Flame> & newFlames);
        void addFlame(const Flame & flame);

        int totalProbability = 0;
        std::map<Position, Bomb> bombs;
        std::map<Position, Flame> flames;
        std::map<Position, Wall> walls; // Note that It is not neccessary to have files for different type of wall, because they are different only in that if they are destoyable by bomb.
        std::map<Position, std::shared_ptr<Booster>> boosters;
        std::map<Position, Empty> emptySpaces;
        std::map<int, Position> playersPositions; 
        std::shared_ptr<Board> board;
        std::vector<std::pair<int, char>> probabilities;
        ScoreSystem scoreSystem;
};