#include "Empty.h"

#include "Object.h"
#include "Position.h"

Empty::Empty(const Position & pos) :
    Object(pos, ' ') {}