#pragma once

#include <string>

/**
 * @brief It holds single record of scoreborad (name and score).
*/
struct ScoreBoard {
        bool operator < (const ScoreBoard & other) const;
        std::string name;
        int score;
};