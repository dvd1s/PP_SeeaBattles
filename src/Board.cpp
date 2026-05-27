#include "Board.hpp"

#include <algorithm>

namespace {

constexpr int kEmpty = -1;

bool allInBounds(const std::vector<Coord>& cells) {
    return std::all_of(cells.begin(), cells.end(), [](Coord c) { return inBounds(c); });
}

}  // namespace

Board::Board() {
    for (int row = 0; row < kBoardSize; ++row) {
        for (int col = 0; col < kBoardSize; ++col) {
            occupancy_[row][col] = kEmpty;
            marks_[row][col] = CellMark::Unknown;
        }
    }
}

bool Board::canPlace(const Ship& candidate) const {
    const auto cells = candidate.cells();
    if (!allInBounds(cells)) {
        return false;
    }
    for (const auto c : cells) {
        if (occupancy_[c.row][c.col] != kEmpty) {
            return false;
        }
    }
    return !hasAdjacentConflict(candidate);
}

bool Board::hasAdjacentConflict(const Ship& candidate) const {
    const auto cells = candidate.cells();
    for (const auto c : cells) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) {
                    continue;
                }
                const Coord neighbor{c.col + dc, c.row + dr};
                if (!inBounds(neighbor)) {
                    continue;
                }
                const int idx = occupancy_[neighbor.row][neighbor.col];
                if (idx == kEmpty) {
                    continue;
                }
                bool partOfCandidate = false;
                for (const auto cc : cells) {
                    if (cc == neighbor) {
                        partOfCandidate = true;
                        break;
                    }
                }
                if (!partOfCandidate) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::placeShip(const Ship& ship) {
    if (!canPlace(ship)) {
        return false;
    }
    const int index = static_cast<int>(ships_.size());
    ships_.push_back(ship);
    for (const auto c : ship.cells()) {
        occupancy_[c.row][c.col] = index;
    }
    return true;
}

ShotResult Board::shoot(Coord c) {
    if (!inBounds(c)) {
        return ShotResult::Already;
    }

    const auto currentMark = marks_[c.row][c.col];
    if (currentMark == CellMark::Miss || currentMark == CellMark::Hit || currentMark == CellMark::Sunk) {
        return ShotResult::Already;
    }

    const int shipIdx = occupancy_[c.row][c.col];
    if (shipIdx == kEmpty || shipIdx < 0 || static_cast<size_t>(shipIdx) >= ships_.size()) {
        marks_[c.row][c.col] = CellMark::Miss;
        return ShotResult::Miss;
    }

    ships_[shipIdx].registerHit(c);
    if (ships_[shipIdx].isSunk()) {
        for (const auto cell : ships_[shipIdx].cells()) {
            marks_[cell.row][cell.col] = CellMark::Sunk;
        }
        markSurroundingAsSunk(shipIdx);
        return ShotResult::Sunk;
    }

    marks_[c.row][c.col] = CellMark::Hit;
    return ShotResult::Hit;
}

void Board::markSurroundingAsSunk(int shipIndex) {
    for (const auto c : ships_[shipIndex].cells()) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                const Coord neighbor{c.col + dc, c.row + dr};
                if (!inBounds(neighbor)) {
                    continue;
                }
                if (marks_[neighbor.row][neighbor.col] == CellMark::Unknown) {
                    marks_[neighbor.row][neighbor.col] = CellMark::Miss;
                }
            }
        }
    }
}

bool Board::allShipsSunk() const {
    if (ships_.empty()) {
        return false;
    }
    return std::all_of(ships_.begin(), ships_.end(), [](const Ship& s) { return s.isSunk(); });
}

int Board::shipsRemaining() const {
    int count = 0;
    for (const auto& ship : ships_) {
        if (!ship.isSunk()) {
            ++count;
        }
    }
    return count;
}

bool Board::placeRandomFleet(const FleetConfig& fleet, const ShipCatalog& catalog, std::mt19937& rng) {
    for (int row = 0; row < kBoardSize; ++row) {
        for (int col = 0; col < kBoardSize; ++col) {
            occupancy_[row][col] = kEmpty;
            marks_[row][col] = CellMark::Unknown;
        }
    }
    ships_.clear();

    const auto queue = fleet.buildPlacementQueue();
    std::uniform_int_distribution<int> colDist(0, kBoardSize - 1);
    std::uniform_int_distribution<int> rowDist(0, kBoardSize - 1);
    std::uniform_int_distribution<int> dirDist(0, 1);

    constexpr int kMaxAttempts = 5000;

    for (const auto& shipId : queue) {
        const ShipDefinition* def = catalog.findById(shipId);
        if (!def) {
            return false;
        }

        bool placed = false;
        for (int attempt = 0; attempt < kMaxAttempts && !placed; ++attempt) {
            const Coord anchor{colDist(rng), rowDist(rng)};
            const Direction dir = dirDist(rng) == 0 ? Direction::Horizontal : Direction::Vertical;
            Ship candidate(*def, anchor, dir);
            if (placeShip(candidate)) {
                placed = true;
            }
        }
        if (!placed) {
            ships_.clear();
            for (int row = 0; row < kBoardSize; ++row) {
                for (int col = 0; col < kBoardSize; ++col) {
                    occupancy_[row][col] = kEmpty;
                }
            }
            return false;
        }
    }
    return true;
}

CellMark Board::markAt(Coord c) const {
    return marks_[c.row][c.col];
}

std::optional<int> Board::shipIndexAt(Coord c) const {
    if (!inBounds(c)) {
        return std::nullopt;
    }
    const int idx = occupancy_[c.row][c.col];
    if (idx == kEmpty || idx < 0 || static_cast<size_t>(idx) >= ships_.size()) {
        return std::nullopt;
    }
    return idx;
}

char Board::displayCharAt(Coord c, bool showFleet) const {
    const auto mark = marks_[c.row][c.col];
    if (mark == CellMark::Miss) {
        return 'o';
    }
    if (mark == CellMark::Hit) {
        return 'x';
    }
    if (mark == CellMark::Sunk) {
        const auto idx = shipIndexAt(c);
        if (idx) {
            return ships_[*idx].sunkChar();
        }
        return '%';
    }

    if (showFleet) {
        const auto idx = shipIndexAt(c);
        if (idx) {
            const auto& ship = ships_[*idx];
            if (ship.isHit(c)) {
                return ship.hitChar();
            }
            return ship.bodyChar();
        }
    }
    return '.';
}
