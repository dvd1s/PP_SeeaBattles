#pragma once

#include "Board.hpp"
#include "FleetConfig.hpp"
#include "ShipCatalog.hpp"

#include <string>

class PlacementPhase {
public:
    PlacementPhase(const FleetConfig& fleet, const ShipCatalog& catalog);

    Board runForPlayer(int playerNumber, const std::string& playerName);

private:
    bool tryPlaceCurrent(Board& board, const std::string& shipId, Coord anchor, Direction dir);
    void renderFrame(
        const Board& board,
        const std::string& playerName,
        const std::string& shipId,
        const ShipDefinition& def,
        Coord cursor,
        Direction dir,
        bool valid) const;

    const FleetConfig& fleet_;
    const ShipCatalog& catalog_;
};
