#pragma once

#include "ShipDefinition.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

class ShipCatalog {
public:
    static ShipCatalog loadFromDirectory(const std::filesystem::path& shipsDir);

    const ShipDefinition* findById(const std::string& id) const;
    const std::vector<ShipDefinition>& all() const { return definitions_; }

private:
    static ShipDefinition loadFromFile(const std::filesystem::path& path);

    std::vector<ShipDefinition> definitions_;
    std::unordered_map<std::string, size_t> index_;
};
