#include "BattlePhase.hpp"

#include "BoardRenderer.hpp"
#include "ConsoleIO.hpp"

#include <iostream>
#include <string>

namespace {

std::string shotResultText(ShotResult result) {
    switch (result) {
    case ShotResult::Miss:
        return "Miss!";
    case ShotResult::Hit:
        return "Hit!";
    case ShotResult::Sunk:
        return "Sunk!";
    case ShotResult::Already:
        return "Already targeted.";
    }
    return "";
}

void renderBattleFrame(
    const std::string& shooterName,
    const Board& targetBoard,
    Coord cursor,
    const std::string& status) {
    ConsoleIO::clearScreen();
    std::cout << "=== " << shooterName << " — fire at enemy waters ===\n";
    std::cout << status << "\n\n";
    BoardRenderer::printTargetingBoard(targetBoard, cursor);
    std::cout << "\nArrows: aim | Enter: fire\n";
}

}  // namespace

int BattlePhase::run(Board& boardPlayer1, Board& boardPlayer2,
                     const std::string& name1, const std::string& name2) {
    int currentPlayer = 1;
    Coord cursor{0, 0};
    std::string status = "Game started. Good luck!";

    while (true) {
        Board& target = currentPlayer == 1 ? boardPlayer2 : boardPlayer1;
        const std::string& currentName = currentPlayer == 1 ? name1 : name2;
        renderBattleFrame(currentName, target, cursor, status);

        const KeyAction key = ConsoleIO::readKey();
        if (key == KeyAction::Quit || key == KeyAction::Escape) {
            throw std::runtime_error("Battle cancelled.");
        }

        if (key == KeyAction::Up && cursor.row > 0) {
            --cursor.row;
            continue;
        }
        if (key == KeyAction::Down && cursor.row < kBoardSize - 1) {
            ++cursor.row;
            continue;
        }
        if (key == KeyAction::Left && cursor.col > 0) {
            --cursor.col;
            continue;
        }
        if (key == KeyAction::Right && cursor.col < kBoardSize - 1) {
            ++cursor.col;
            continue;
        }

        if (key != KeyAction::Enter) {
            continue;
        }

        const ShotResult result = target.shoot(cursor);
        if (result == ShotResult::Already) {
            status = shotResultText(result);
            continue;
        }

        status = shotResultText(result) + " Ships left: " + std::to_string(target.shipsRemaining());

        if (target.allShipsSunk()) {
            ConsoleIO::clearScreen();
            std::cout << "=== BATTLE OVER ===\n\n";
            std::cout << currentName << " wins!\n\n";
            ConsoleIO::waitForAnyKey("Press any key to see results...");
            return currentPlayer;
        }

        if (result == ShotResult::Miss) {
            currentPlayer = currentPlayer == 1 ? 2 : 1;
            const std::string& nextName = currentPlayer == 1 ? name1 : name2;
            ConsoleIO::pauseBetweenPlayers(nextName);
        } else {
            status += " (extra turn)";
        }
    }
}
