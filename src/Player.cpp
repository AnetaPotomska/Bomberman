#include "Player.h"

#include <ncurses.h>
#include <memory>

#include "Object.h"
#include "Map.h"
#include "Board.h"
#include "Position.h"
#include "Statistics.h"
#include "Direction.h"
#include "../libs/json.hpp"

Player::Player(const Position & pos, int id, std::shared_ptr<Map> map, std::shared_ptr<Board> board, const nlohmann::json & playersStats) :
    Object(pos, id + '0'),
    id(id),
    map(map),
    board(board),
    stats(id, playersStats) {}

int Player::getId() const {
    return id;
}

int Player::getScore() const {
    return stats.score;
}

Statistics & Player::getStats() {
    return stats;
}

bool Player::isAlive() const {
    return stats.isAlive();
}

void Player::updateLives(int add) {
    stats.updateLives(add);
}

void Player::updateBombs(int add) {
    stats.updateBombs(add);
}

void Player::updateScore(int add) {
    stats.updateScore(add);
}

bool Player::move(int direction) {
    Position posTmp = pos;
    switch(direction) {
        case LEFT:
            posTmp.x = pos.x - 1;
            break;
        case RIGHT:
            posTmp.x = pos.x + 1;
            break;
        case UP:
            posTmp.y = pos.y - 1;
            break;
        case DOWN:
            posTmp.y = pos.y + 1;
            break;
        default:
            throw("Case for different direction.");
    }
    if(map->isAbleToMove(posTmp) || (stats.hasBombPass && map->isOnOwnBomb(posTmp, id))) {
        Position oldPos = pos;
        pos = posTmp;
        board->draw(pos, icon);
        board->draw(oldPos, ' ');
        return true;
    }
    return false;
}

void Player::show() {
    if(isAlive()) {
        board->draw(pos, icon);
    }
}

void Player::refreshInfoTable() {
    stats.refreshInfoTable();
}

void Player::clearInfoTable() {
    stats.clearInfoTable();
}

bool Player::placeBomb() {
    if(stats.bombsAvailable > 0) {
        stats.updateBombs(-1);
        map->addBomb(Bomb(pos, stats.counterOnBombs, id, stats.flamesPower));
        return true;
    }
    return false;
}

bool Player::remotelyExplodeBombs() {
    if(stats.hasRemoteControl && stats.bombsPlaced > 0) {
        stats.updateBombs(stats.bombsPlaced);
        stats.updateScore(map->detonateById(id));
        return true;
    }
    return false;
}