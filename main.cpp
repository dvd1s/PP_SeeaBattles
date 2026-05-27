#include "AssetPaths.hpp"
#include "FleetConfig.hpp"
#include "Game.hpp"
#include "ShipCatalog.hpp"

#include <iostream>

int main() {
    try {
        const auto fleet = FleetConfig::loadFromFile(AssetPaths::fleetConfigFile().string());
        const auto catalog = ShipCatalog::loadFromDirectory(AssetPaths::shipsDirectory());
        Game game(fleet, catalog);
        game.run();
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
