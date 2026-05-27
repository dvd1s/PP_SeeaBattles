#include "PlacementPhase.hpp"

#include "BoardRenderer.hpp"
#include "ConsoleIO.hpp"

#include <iostream>
#include <random>
#include <stdexcept>

PlacementPhase::PlacementPhase(const FleetConfig& fleet, const ShipCatalog& catalog)
    : fleet_(fleet), catalog_(catalog) {}

bool PlacementPhase::tryPlaceCurrent(
    Board& board,
    const std::string& shipId,
    Coord anchor,
    Direction dir) {
    const ShipDefinition* def = catalog_.findById(shipId);
    if (!def) {
        return false;
    }
    Ship candidate(*def, anchor, dir);
    return board.placeShip(candidate);
}

void PlacementPhase::renderFrame(
    const Board& board,
    const std::string& playerName,
    const std::string& shipId,
    const ShipDefinition& def,
    Coord cursor,
    Direction dir,
    bool valid) const {
    ConsoleIO::clearScreen();
    std::cout << "=== " << playerName << " — place your fleet ===\n";
    std::cout << "Placing: " << def.name << " (" << shipId << ")\n";
    std::cout << "Arrows: move | Space: rotate | Enter: confirm | R: random fleet\n\n";
    BoardRenderer::printOwnBoard(board, catalog_, cursor, def, dir, valid);
    std::cout << '\n';
    if (!valid) {
        std::cout << "Invalid position (overlap or ships touching).\n";
    }
}

Board PlacementPhase::runForPlayer(int playerNumber, const std::string& playerName) {
    ConsoleIO::pauseBetweenPlayers(playerName);

    Board board;
    auto queue = fleet_.buildPlacementQueue();
    std::mt19937 rng(static_cast<unsigned>(playerNumber * 9973 + 42));

    size_t shipIndex = 0;
    Coord cursor{0, 0};
    Direction dir = Direction::Horizontal;

    while (shipIndex < queue.size()) {
        const std::string& shipId = queue[shipIndex];
        const ShipDefinition* def = catalog_.findById(shipId);
        if (!def) {
            throw std::runtime_error("Unknown ship in fleet: " + shipId);
        }

        Ship preview(*def, cursor, dir);
        bool valid = board.canPlace(preview);
        renderFrame(board, playerName, shipId, *def, cursor, dir, valid);

        const KeyAction key = ConsoleIO::readKey();
        switch (key) {
        case KeyAction::Up:
            if (cursor.row > 0) {
                --cursor.row;
            }
            break;
        case KeyAction::Down:
            if (cursor.row < kBoardSize - 1) {
                ++cursor.row;
            }
            break;
        case KeyAction::Left:
            if (cursor.col > 0) {
                --cursor.col;
            }
            break;
        case KeyAction::Right:
            if (cursor.col < kBoardSize - 1) {
                ++cursor.col;
            }
            break;
        case KeyAction::Rotate:
            dir = flip(dir);
            break;
        case KeyAction::Enter:
            if (tryPlaceCurrent(board, shipId, cursor, dir)) {
                ++shipIndex;
                cursor = {0, 0};
                dir = Direction::Horizontal;
            }
            break;
        case KeyAction::RandomFleet: {
            Board randomBoard;
            if (randomBoard.placeRandomFleet(fleet_, catalog_, rng)) {
                board = std::move(randomBoard);
                shipIndex = queue.size();
            } else {
                ConsoleIO::clearScreen();
                std::cout << "Could not generate a random fleet. Try again.\n";
                ConsoleIO::waitForAnyKey("Press any key...");
            }
            break;
        }
        case KeyAction::Escape:
        case KeyAction::Quit:
            throw std::runtime_error("Placement cancelled.");
        default:
            break;
        }
    }

    ConsoleIO::clearScreen();
    std::cout << playerName << " fleet ready.\n";
    BoardRenderer::printOwnBoard(board, catalog_, {0, 0}, std::nullopt, Direction::Horizontal, true);
    ConsoleIO::waitForAnyKey("\nPress any key to hide your fleet...");
    return board;
}
