#pragma once

#include "Object.h"
#include "Position.h"

/**
 * @brief Flame is object represented by x symbol.
 * When bomb explodes it will spawn flames.
 * Flames can destroy destroyable walls and kill players.
*/
class Flame : public Object {
    public:
        Flame() = default;
        Flame(const Position & pos, int id);
        int getId() const;
    private:
        int playerId; // which player created this flame, respectively his bomb
};