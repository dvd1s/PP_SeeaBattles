#include "AssetPaths.hpp"

#include <windows.h>

namespace AssetPaths {

std::filesystem::path executableDirectory() {
    wchar_t buffer[MAX_PATH]{};
    const DWORD len = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    std::filesystem::path exePath(buffer, buffer + len);
    return exePath.parent_path();
}

std::filesystem::path assetsRoot() {
    return executableDirectory() / "assets";
}

std::filesystem::path shipsDirectory() {
    return assetsRoot() / "ships";
}

std::filesystem::path fleetConfigFile() {
    return assetsRoot() / "fleet.txt";
}

}  // namespace
