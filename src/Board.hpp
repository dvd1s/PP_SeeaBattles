#pragma once

#include "CellMark.hpp"
#include "Coord.hpp"
#include "FleetConfig.hpp"
#include "Ship.hpp"
#include "ShipCatalog.hpp"

#include <optional>
#include <random>
#include <vector>

class Board {
public:
    Board();

    bool canPlace(const Ship& candidate) const;
    bool placeShip(const Ship& ship);
    ShotResult shoot(Coord c);
    bool allShipsSunk() const;
    int shipsRemaining() const;

    bool placeRandomFleet(const FleetConfig& fleet, const ShipCatalog& catalog, std::mt19937& rng);

    const std::vector<Ship>& ships() const { return ships_; }
    CellMark markAt(Coord c) const;
    std::optional<int> shipIndexAt(Coord c) const;
    char displayCharAt(Coord c, bool showFleet) const;

private:
    bool hasAdjacentConflict(const Ship& candidate) const;
    void markSurroundingAsSunk(int shipIndex);

    int occupancy_[kBoardSize][kBoardSize]{};  // -1 empty, else ship index
    CellMark marks_[kBoardSize][kBoardSize]{};
    std::vector<Ship> ships_;
};
