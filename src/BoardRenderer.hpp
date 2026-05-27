#pragma once

#include "Board.hpp"
#include "Coord.hpp"
#include "ShipCatalog.hpp"
#include "ShipDefinition.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <vector>

class BoardRenderer {
public:
    static void printTargetingBoard(const Board& board, Coord cursor);
    static void printOwnBoard(
        const Board& board,
        const ShipCatalog& catalog,
        Coord cursor,
        const std::optional<ShipDefinition>& previewDef,
        Direction previewDir,
        bool previewValid);

private:
    static std::vector<std::string> buildDisplayGrid(
        const Board& board,
        bool showFleet,
        const ShipCatalog* catalog,
        const std::optional<ShipDefinition>& previewDef,
        Coord previewAnchor,
        Direction previewDir,
        bool previewValid);

    static void printGrid(const std::vector<std::string>& grid, Coord cursor, bool showCursor);
    static char columnLabel(int col);
};
