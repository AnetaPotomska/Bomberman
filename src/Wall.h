#pragma once

#include "Object.h"
#include "Position.h"

/**
 * @brief Represents wall.
 * Destroyable wall is represented by + symbol.
 * Undestructable wall is represented by # symbol.
*/
class Wall : public Object {
    public:
        Wall() = default;
        Wall(const Position & pos, char symbol, bool destroyable);
        bool isDestroyable() const;
    private:
        bool destroyable; 
};