#pragma once

#include <vector>

struct Position {
    Position() = default;
    Position(int y, int x);
    Position(const Position & other);
    Position & operator = (Position other);
    ~Position () = default;
    void swap (Position & other);
    bool operator < (const Position & other) const;
    bool operator == (const Position & other) const;
    bool operator != (const Position & other) const;
    std::vector<Position> getAround() const;
    int y;
    int x;
};