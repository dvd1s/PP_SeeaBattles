#pragma once

#include "FleetConfig.hpp"
#include "ShipCatalog.hpp"

#include <filesystem>
#include <string>

class Game {
public:
    Game(FleetConfig fleet, ShipCatalog catalog);

    void run();

private:
    void promptPlayerNames();
    void saveResultToCsv(const std::string& winnerName);
    void showSessionScore() const;

    FleetConfig fleet_;
    ShipCatalog catalog_;
    std::string player1Name_;
    std::string player2Name_;
    int wins1_ = 0;
    int wins2_ = 0;
    int gamesPlayed_ = 0;
    std::filesystem::path csvPath_;
};
