#include "Game.hpp"

#include "AssetPaths.hpp"
#include "BattlePhase.hpp"
#include "ConsoleIO.hpp"
#include "PlacementPhase.hpp"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

Game::Game(FleetConfig fleet, ShipCatalog catalog)
    : fleet_(std::move(fleet)), catalog_(std::move(catalog)) {}

static std::string currentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    localtime_s(&tm, &t);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    return buf;
}

void Game::promptPlayerNames() {
    ConsoleIO::clearScreen();
    std::cout << "=== PLAYER SETUP ===\n\n";
    std::cout << "Enter player names (press Enter to use default).\n\n";

    player1Name_ = ConsoleIO::readLine("Player 1 name (default: Player 1): ");
    if (player1Name_.empty()) player1Name_ = "Player 1";

    player2Name_ = ConsoleIO::readLine("Player 2 name (default: Player 2): ");
    if (player2Name_.empty()) player2Name_ = "Player 2";

    std::cout << "\nWelcome, " << player1Name_ << " and " << player2Name_ << "!\n";
    ConsoleIO::waitForAnyKey("Press any key to start...");
}

void Game::saveResultToCsv(const std::string& winnerName) {
    bool writeHeader = !std::filesystem::exists(csvPath_);
    std::ofstream file(csvPath_, std::ios::app);
    if (!file) return;
    if (writeHeader) {
        file << "Timestamp,Player1,Player2,Winner\n";
    }
    file << currentTimestamp() << ","
         << player1Name_ << ","
         << player2Name_ << ","
         << winnerName << "\n";
}

void Game::showSessionScore() const {
    std::cout << "=== SESSION SCORE ===\n\n";
    std::cout << "  " << player1Name_ << ": " << wins1_ << " win(s)\n";
    std::cout << "  " << player2Name_ << ": " << wins2_ << " win(s)\n";
    std::cout << "  Games played: " << gamesPlayed_ << "\n";
}

void Game::run() {
    ConsoleIO::hideCursor();
    ConsoleIO::clearScreen();
    std::cout << "=== SEA BATTLE ===\n";
    std::cout << "Hot-seat duel on a 10x10 grid.\n";
    std::cout << "Place ships without touching. Hits grant another shot.\n\n";
    ConsoleIO::waitForAnyKey("Press any key to start...");

    promptPlayerNames();
    csvPath_ = AssetPaths::executableDirectory() / "battles.csv";

    while (true) {
        PlacementPhase placement(fleet_, catalog_);
        Board board1 = placement.runForPlayer(1, player1Name_);
        Board board2 = placement.runForPlayer(2, player2Name_);

        BattlePhase battle;
        int winner = battle.run(board1, board2, player1Name_, player2Name_);

        ++gamesPlayed_;
        const std::string& winnerName = winner == 1 ? player1Name_ : player2Name_;
        if (winner == 1) ++wins1_; else ++wins2_;

        saveResultToCsv(winnerName);

        ConsoleIO::clearScreen();
        showSessionScore();
        std::cout << "\nResults saved to: battles.csv\n\n";

        ConsoleIO::showCursor();
        std::cout << "Play again? (Y/N): ";
        std::string answer;
        std::getline(std::cin, answer);
        ConsoleIO::hideCursor();

        if (answer.empty() || (answer[0] != 'y' && answer[0] != 'Y')) {
            break;
        }
    }

    ConsoleIO::showCursor();
}
