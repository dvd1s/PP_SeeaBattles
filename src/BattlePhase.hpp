#pragma once

#include "Board.hpp"

#include <string>

class BattlePhase {
public:
    int run(Board& boardPlayer1, Board& boardPlayer2,
            const std::string& name1, const std::string& name2);
};
