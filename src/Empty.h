#pragma once

#include "Object.h"
#include "Position.h"

/**
 * @brief Represents empty spot in map.
 * Is reperesented by ' ' symbol.
*/
class Empty : public Object {
    public:
        Empty() = default;
        Empty(const Position & pos);
};