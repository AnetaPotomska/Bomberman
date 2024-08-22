#pragma once

#include <string>
#include <memory>
#include <vector>

#include "ConfigurationManager.h"
#include "Position.h"
#include "Board.h"
#include "ScoreSystem.h"
#include "Map.h"
#include "Player.h"

/**
 * @brief Game is controller. Main purpose is to run game loop.
 * Game has overview about players and object in map.
*/
class Game {
    public:
        Game(int gameMode, const std::string & levelName, const ConfigurationManager & conf);
        /**
         * @param playerType indicates if player is Human of PC.
         * It will add players based on gameMode. When gameMode is HUMAN_VS_HUMAN all players are human.
         * Otherwise only one player is human and others are PC players.
         * Game will add PC players from least difficul to hardest. 
        */
        void addPlayer(const Position & pos, int playerType);
        /**
         * @brief Add object to map.
         * Now there are only 3 object that can exists on map (exludes player):
            * # undestroyable wall
            * + destroyable wall
            * _ empty space 
        */
        void addObject(const Position & pos, char symbol);
        /**
         * @brief Based on level size it will initilize Board size.
        */
        void initilizeBoard(int numOfRows, int numOfCols);
        /**
         * @brief It will initilize map, show everything (map, players, info tables), 
         * start game loop, clean up afterwards and then asks human players for theirs names.
         * So game can store their scores in scoreboard of level and gameMode.
        */
        void startGame();
    private:
        /**
         * @brief It will run window, where human players can enter their names. 
         * Then his name and score will be saved to corresponding scoreboard (based on level played and gamemode).
        */
        void getName();
        void startGameLoop();
        /**
         * @brief Determines whose turn it is (which player or bombs). 
         * If bombs it will detonate them (and give players score and back his bombs). 
         * If player then he can move, place bomb or detonates all of his bombs if he has remote controller.
        */
        void processInput();
        /**
         * @brief Main purpose is deal with collisions:
        * If player is colliding with flame he will loose life, and
        * player who killed him will get points (only if these players are not
        * the same person, then player would get penalization score)
        * 
        * If player is colliding with bonus he will pick it up.
        * 
        * If there is only 1 or 0 palyers alive game ends.
        */
		void updateState();
        /**
         * @brief It will redraw windows for board and for info tabs.
        */
		void redraw();
        /**
         * @brief It will show and refresh everything.
        */
        void showEverything();

        std::shared_ptr<Board> board; // is for drawing purposes
        bool isUp = true; // check if game is over
        std::vector<std::shared_ptr<Player>> players; // list of active players
        std::shared_ptr<Map> map; // map (includes all object besides players)
        int playerOnTurn = 0; // for turn-based game
        int gameMode;
        std::string levelName;
        int humanPlayers = 0; // need to know that so we can ask for their names, human players are always players next to each other from 0 id
        int playersAlive = 0;
        int PCPlayers = 0;
        ConfigurationManager conf;
        ScoreSystem scoreSystem;
};