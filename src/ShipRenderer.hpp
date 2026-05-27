#pragma once

#include "Coord.hpp"
#include "Direction.hpp"
#include "ShipDefinition.hpp"

#include <string>
#include <vector>

class ShipRenderer {
public:
    static void drawOntoGrid(
        std::vector<std::string>& grid,
        Coord anchor,
        Direction direction,
        const ShipDefinition& def,
        char overlayChar,
        bool validPlacement);
};
