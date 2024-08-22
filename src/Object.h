#pragma once

#include <ncurses.h>

#include "Position.h"

/**
 * @brief Object only holds position and printable icon.
*/
class Object {
    public:
        Object() = default;
        Object(const Position & pos, chtype icon);
        Position getPos() const;
        chtype getIcon() const;
    protected:
        Position pos;
        chtype icon;
};