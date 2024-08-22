#include "ScoreBoard.h"

bool ScoreBoard::operator < (const ScoreBoard & other) const {
    return score > other.score; 
}