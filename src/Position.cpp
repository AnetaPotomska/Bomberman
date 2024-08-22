#include "Position.h"

#include <vector>
#include <algorithm>

Position::Position(int y, int x) :
    y(y),
    x(x) {}

bool Position::operator < (const Position & other) const{
    if(y == other.y) {
        return x > other.x;
    }
    return y > other.y;
}

bool Position::operator == (const Position & other) const {
    return x == other.x && y == other.y;
}

bool Position::operator != (const Position & other) const {
    return x != other.x || y != other.y;
}

std::vector<Position> Position::getAround() const {
    std::vector<Position> around;
    around.push_back(Position(y - 1, x));
    around.push_back(Position(y + 1, x));
    around.push_back(Position(y, x - 1));
    around.push_back(Position(y, x + 1));
    return around;
}


Position::Position (const Position & other) {
  y = other.y;
  x = other.x;
}

Position & Position::operator = (Position other) {
  swap(other);
  return *this;
}

void Position::swap (Position & other) {
  std::swap(y, other.y);
  std::swap(x, other.x);
}