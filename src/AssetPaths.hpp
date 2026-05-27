#pragma once

#include <filesystem>

namespace AssetPaths {
    std::filesystem::path executableDirectory();
    std::filesystem::path assetsRoot();
    std::filesystem::path shipsDirectory();
    std::filesystem::path fleetConfigFile();
}
