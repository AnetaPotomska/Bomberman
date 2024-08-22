#include "Object.h"

#include <ncurses.h>

#include "Position.h"

Object::Object(const Position & pos, chtype icon) :
    pos(pos),
    icon(icon) {}

Position Object::getPos() const {
    return pos;
}

chtype Object::getIcon() const {
    return icon;
}