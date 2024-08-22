#include "BoxLoverPC.h"

#include <memory>
#include <vector>

#include "PCPlayer.h"
#include "Map.h"
#include "Board.h"
#include "Position.h"
#include "../libs/json.hpp"

BoxLoverPC::BoxLoverPC(const Position & pos, int id, std::shared_ptr<Map> map, std::shared_ptr<Board> board, const nlohmann::json & playersStats) :
    PCPlayer(pos, id, map, board, playersStats) {}

void BoxLoverPC::doTurn() {
    if(commitSuicide) {
        placeBomb();
        return;
    }
    if(!takingCover && iWillBeHit()) {
        takeCover();
        takingCover = true;
        goalIsTargeted = false;
    }
    if(takingCover) {
        if(pathToGetToCover.empty()) {
            takingCover = false;
        }
        else {
            goTowardsGoal(pathToGetToCover);
        }
    }
    else if(!takingCover) {
        if(goalIsTargeted && targetWasDestroyed()) {
            goalIsTargeted = false;
        }
        if(!goalIsTargeted) {
            Position newTarget;
            std::deque<Position> newPath;
            if(!targetGoal(map->getDestroyableWallsPositions(), newTarget, newPath)) {
                placeBomb();
                commitSuicide = true;
                return;
            }
            else {
                target = newTarget;
                pathToGetToGoal = newPath;
                goalIsTargeted = true;
            }
        }
        if(goalIsTargeted) {
            if(stats.hasRemoteControl && !iWillBeHit()) {
                if(remotelyExplodeBombs()) {
                    goalIsTargeted = false;
                    return;
                }
            }
            if(targetWouldBeHit()) {
                if(placeBomb()) {
                    goalIsTargeted = false;
                    return;
                }
            }
            goTowardsGoal(pathToGetToGoal);
        }
    }
}

std::vector<Bomb> BoxLoverPC::getBombs() const {
    return map->findBombById(id);
}
