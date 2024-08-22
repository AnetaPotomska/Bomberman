#include "HumanPlayer.h"

#include <ncurses.h>
#include <memory>
#include <ctime>
#include <string>

#include "Player.h"
#include "Map.h"
#include "Board.h"
#include "Position.h"
#include "Direction.h"
#include "../libs/json.hpp"
#include "Time.h"

HumanPlayer::HumanPlayer(const Position & pos, int id, std::shared_ptr<Map> map, std::shared_ptr<Board> board, const nlohmann::json & playersStats, const nlohmann::json & keysJson) :
    Player(pos, id, map, board, playersStats) {
        parseKeysFromJson(keysJson);
    }

void HumanPlayer::doTurn() {
    Time t;
    time_t timeLastInput = t.milliseconds();
    setTimeout(0);
    int fix = 500;
    while(timeLastInput + (stats.speed * fix) >= t.milliseconds()) {
        if(proccessInput(getInput())) {
            return;
        }
    }
    setTimeout(stats.speed * fix);
}

void HumanPlayer::parseKeysFromJson(const nlohmann::json & keysJson) {
    if(!keysJson.contains("up") || !keysJson.contains("down") || !keysJson.contains("left") || !keysJson.contains("right") || !keysJson.contains("place_bomb") || !keysJson.contains("remote_control")) {
        throw("Some key from player config is missing.");
    }
    keyUp = keysJson["up"].get<std::string>()[0];
    keyDown = keysJson["down"].get<std::string>()[0];
    keyLeft = keysJson["left"].get<std::string>()[0];
    keyRight = keysJson["right"].get<std::string>()[0];
    keyPlaceBomb = keysJson["place_bomb"].get<std::string>()[0];
    keyRemoteControlExplodeBomb = keysJson["remote_control"].get<std::string>()[0];
}

chtype HumanPlayer::getInput() {
    return wgetch(board->getBoardWin().get());
}

bool HumanPlayer::proccessInput(chtype input) {
    if(input == keyUp) {
        return move(UP);
    }
    else if(input == keyDown) {
        return move(DOWN);
    }
    else if(input == keyLeft) {
        return move(LEFT);
    }
    else if(input == keyRight) {
        return move(RIGHT);
    }
    else if(input == keyPlaceBomb) {
        return placeBomb();
    }
    else if(input == keyRemoteControlExplodeBomb) {
        return remotelyExplodeBombs();
    }
    return false;
}

void HumanPlayer::setTimeout(int t) {
    wtimeout(board->getBoardWin().get(), t);
}