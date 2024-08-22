#include "PCPlayer.h"

#include <set>
#include <map>
#include <deque>
#include <vector>
#include <algorithm>

#include "Player.h"
#include "Position.h"
#include "Empty.h"
#include "Map.h"
#include "Board.h"
#include "Direction.h"
#include "../libs/json.hpp"

PCPlayer::PCPlayer(const Position & pos, int id, std::shared_ptr<Map> map, std::shared_ptr<Board> board, const nlohmann::json & playersStats) :
    Player(pos, id, map, board, playersStats),
    target(0,0) {}


bool PCPlayer::findPath(const std::map<Position, std::set<Position>> & graph, const Position & start, const Position & end, std::deque<Position> & path) {
    std::queue<Position> bfs;
    std::set<Position> visited;
    bfs.push(start);
    visited.insert(start);
    std::map<Position, Position> P;
    while(!bfs.empty()) {
        Position tmpPos = bfs.front();
        bfs.pop();
        if(tmpPos == end) {
            break;
        }
        for(auto & x : graph.at(tmpPos)) {
            if(visited.count(x) == 0) { 
                bfs.push(x);
                visited.insert(x);
                P[x] = tmpPos;
            }
        }
    }
    if(end != start)
        path.push_back(end);
    Position tmpPos = end;
    while (P.count(tmpPos)) {
        tmpPos = P[tmpPos];
        if(tmpPos != start)
            path.push_back(tmpPos);
    }
    std::reverse(path.begin(), path.end());
    return visited.count(end); // true we found end
}

void PCPlayer::addToGraph(std::map<Position, std::set<Position>> & graph, const std::map<Position, Empty> & emptySpaces, const Position & posToAdd) {
    std::set<Position> neighbours;
    std::vector<Position> around = posToAdd.getAround();
    for(auto & x : around) {
        if(emptySpaces.count(x)) {
            neighbours.insert(x);
        }
    }
    graph[posToAdd] = neighbours;
}

std::map<Position, std::set<Position>> PCPlayer::toGraph (std::map<Position, Empty> emptySpaces) {
    std::map<Position, std::set<Position>> graph;
    for(auto & x : emptySpaces) {
        addToGraph(graph, emptySpaces, x.first);
    }
    return graph;
}

void PCPlayer::goTowardsGoal(std::deque<Position> & path) {
    if(path.empty()) {
        return;
    }
    Position toGo = path.front();
    path.pop_front();
    if(toGo.x < pos.x) { 
        move(LEFT);
    }
    else if(toGo.x > pos.x) {
        move(RIGHT);
    }
    else if(toGo.y < pos.y) {
        move(UP);
    }
    else if(toGo.y > pos.y) {
        move(DOWN);
    }
    else {
        throw("Cannot continue in path.");
    }
}

bool PCPlayer::targetGoal(const std::vector<Position> & targets, Position & newTarget, std::deque<Position> & newPath) {
    newPath.clear();
    if(targets.size() == 0) {
        return false;
    }
    size_t smallestDistance = 0;
    bool wasSetup = false;
    bool canBeTargeted = false;
    auto emptySpaces = map->getEmptySpaces();
    auto graph = toGraph(emptySpaces);
    std::set<Position> neighbours;
    for(auto it = targets.begin(); it != targets.end(); it++) {
        if(target != *it) {
            std::vector<Position> aroundPos = it->getAround();
            for(auto & x : aroundPos) {
                std::deque<Position> path;
                if(graph.count(x) != 0) {
                    if(findPath(graph, pos, x, path)  && (!wasSetup || smallestDistance > path.size())) {
                        canBeTargeted = true;
                        smallestDistance = path.size();
                        newTarget = *it;
                        newPath = path;
                        wasSetup = true;
                    }
                }
            }
        }
    }
    return canBeTargeted;
}

void PCPlayer::takeCover() {
    auto bombs = getBombs();
    std::map<Position, Flame> allFlames;
    for(auto & x : bombs) {
        auto flames = x.explode(map->getWalls());
        for(auto & x : flames) {
            allFlames[x.first] = x.second;
        }
    }
    size_t smallestDistance = 0;
    bool wasSetup = false;
    auto emptySpaces = map->getEmptySpaces();
    auto graph = toGraph(emptySpaces); // go anywhere in emptyspace that is not in 
    for(auto & x : emptySpaces) {
        if(allFlames.count(x.first) == 0) {
            std::deque<Position> path;
            if(findPath(graph, pos, x.first, path) && (!wasSetup || smallestDistance > path.size())) {
                smallestDistance = path.size();
                pathToGetToCover = path;
                wasSetup = true;
            }
        }
    }
}

bool PCPlayer::iWillBeHit() {
    auto bombs = getBombs();
    for(auto & x : bombs) {
        auto flames = x.explode(map->getWalls());
        if(flames.count(pos)) {
            return true;
        }
    }
    return false;
}

bool PCPlayer::targetWouldBeHit() { // tries if placing the bomb now will result in destroying the box
    Bomb bomb(pos, stats.counterOnBombs, id, stats.flamesPower);
    auto flames = bomb.explode(map->getWalls());
    return flames.count(target);
}


bool PCPlayer::targetWasDestroyed() {
    return !map->doesBoxExists(target);
}
