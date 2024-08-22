#include "ScoreLoverPC.h"

#include <vector>
#include <memory>

#include "PCPlayer.h"
#include "Map.h"
#include "Board.h"
#include "Position.h"
#include "ScoreSystem.h"
#include "../libs/json.hpp"
#include "PossibleTargets.h"

ScoreLoverPC::ScoreLoverPC(const Position & pos, int id, std::shared_ptr<Map> map, std::shared_ptr<Board> board, const nlohmann::json & playersStats, const ScoreSystem & scoreSystem) :
    PCPlayer(pos, id, map, board, playersStats),
    scoreSystem(scoreSystem) {}

void ScoreLoverPC::doTurn() {
    if(!takingCover && iWillBeHit()) {
        takeCover();
        takingCover = true;
        goalIsTargeted = false;
    }
    if(takingCover) {
        pathToGetToGoal.clear();
        if(pathToGetToCover.empty()) {
            takingCover = false;
        }
        else {
            goTowardsGoal(pathToGetToCover);
        }
    }
    else if(!takingCover) {
        if(targetIsBonus && pathToGetToGoal.empty()) {
            goalIsTargeted = false;
        }
        if(!goalIsTargeted) {
            if(!decideOnGoal()) {
                placeBomb();
                return;
            }
            else {
                goalIsTargeted = true;
            }
        }
        if(goalIsTargeted) {
            if(targetIsBonus) {
                goTowardsGoal(pathToGetToGoal);
                return;
            }
            if(stats.hasRemoteControl && !iWillBeHit() && targetWouldBeHit()) {
                if(remotelyExplodeBombs()) {
                    goalIsTargeted = false;
                    targetIsBonus = false;
                    return;
                }
            }
            if(targetWouldBeHit()) {
                if(placeBomb()) {
                    goalIsTargeted = false;
                    targetIsBonus = false;
                    return;
                }
            }
            goTowardsGoal(pathToGetToGoal);
        }
    }
}

bool ScoreLoverPC::decideOnGoal() {
    auto whereToGo = BOMB;
    Position newTarget;
    std::deque<Position> newPath;
    if(scoreSystem.destroyingWall > scoreSystem.placingBomb && targetGoal(map->getDestroyableWallsPositions(), newTarget, newPath)) {
        whereToGo = BOX;
        target = newTarget;
        pathToGetToGoal = newPath;
    }
    if(((whereToGo == BOMB && scoreSystem.pickingUpBonus > scoreSystem.placingBomb) || 
        (whereToGo == BOX && scoreSystem.pickingUpBonus > scoreSystem.destroyingWall)) && 
        targetGoal(map->getBoostersPositions(), newTarget, newPath)) {
            whereToGo = BONUS;
            target = newTarget;
            pathToGetToGoal = newPath;
    }
    if(((whereToGo == BOMB && scoreSystem.killingOtherPlayer > scoreSystem.placingBomb) || 
        (whereToGo == BOX && scoreSystem.killingOtherPlayer > scoreSystem.destroyingWall) || 
        (whereToGo == BONUS && scoreSystem.killingOtherPlayer > scoreSystem.pickingUpBonus)) && 
        targetGoal(map->getPlayersPositionsExceptSelf(id), newTarget, newPath)) {
            whereToGo = PLAYER;
            target = newTarget;
            pathToGetToGoal = newPath;
    }
    if(whereToGo == BONUS) {
        targetIsBonus = true;
        pathToGetToGoal.push_back(target);
    }
    else {
        targetIsBonus = false;
    }
    if(whereToGo == BOMB) {
        targetIsBonus = false;
        return false;
    }
    return true;
}

std::vector<Bomb> ScoreLoverPC::getBombs() const {
    return map->getBombs();
}
