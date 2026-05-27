#include "ShipCatalog.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {

std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    return s.substr(start, end - start);
}

enum class ParseSection { None, Meta, H, V };

}  // namespace

ShipCatalog ShipCatalog::loadFromDirectory(const std::filesystem::path& shipsDir) {
    if (!std::filesystem::is_directory(shipsDir)) {
        throw std::runtime_error("Ships directory not found: " + shipsDir.string());
    }

    ShipCatalog catalog;
    for (const auto& entry : std::filesystem::directory_iterator(shipsDir)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".txt") {
            continue;
        }
        auto def = loadFromFile(entry.path());
        if (catalog.index_.count(def.id) > 0) {
            throw std::runtime_error("Duplicate ship id: " + def.id);
        }
        catalog.index_[def.id] = catalog.definitions_.size();
        catalog.definitions_.push_back(std::move(def));
    }

    if (catalog.definitions_.empty()) {
        throw std::runtime_error("No ship definitions found in: " + shipsDir.string());
    }
    return catalog;
}

ShipDefinition ShipCatalog::loadFromFile(const std::filesystem::path& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Cannot open ship file: " + path.string());
    }

    ShipDefinition def;
    ParseSection section = ParseSection::Meta;
    std::string line;
    int lineNo = 0;

    auto validateSprites = [&]() {
        if (def.spriteH.empty() || def.spriteV.empty()) {
            throw std::runtime_error(path.string() + ": missing [h] or [v] sprite section");
        }
        if (static_cast<int>(def.spriteH.size()) != 1) {
            throw std::runtime_error(path.string() + ": [h] must have exactly 1 line");
        }
        if (static_cast<int>(def.spriteH[0].size()) != def.length) {
            throw std::runtime_error(path.string() + ": [h] width must match length");
        }
        if (static_cast<int>(def.spriteV.size()) != def.length) {
            throw std::runtime_error(path.string() + ": [v] must have length lines");
        }
        for (const auto& row : def.spriteV) {
            if (static_cast<int>(row.size()) != 1) {
                throw std::runtime_error(path.string() + ": [v] lines must be 1 character wide");
            }
        }
    };

    while (std::getline(in, line)) {
        ++lineNo;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        line = trim(line);
        if (line.empty() || (line.size() >= 2 && line[0] == '/' && line[1] == '/')) {
            continue;
        }

        if (line == "[h]") {
            section = ParseSection::H;
            continue;
        }
        if (line == "[v]") {
            section = ParseSection::V;
            continue;
        }

        if (section == ParseSection::Meta) {
            const auto eq = line.find('=');
            if (eq == std::string::npos) {
                throw std::runtime_error(path.string() + ":" + std::to_string(lineNo) + ": invalid line");
            }
            const std::string key = line.substr(0, eq);
            const std::string value = line.substr(eq + 1);
            if (key == "id") {
                def.id = value;
            } else if (key == "name") {
                def.name = value;
            } else if (key == "length") {
                def.length = std::stoi(value);
            } else if (key == "body") {
                def.bodyChar = value.empty() ? '#' : value[0];
            } else if (key == "hit") {
                def.hitChar = value.empty() ? 'X' : value[0];
            } else if (key == "sunk") {
                def.sunkChar = value.empty() ? '%' : value[0];
            }
        } else if (section == ParseSection::H) {
            def.spriteH.push_back(line);
        } else if (section == ParseSection::V) {
            def.spriteV.push_back(line);
        }
    }

    if (def.id.empty() || def.length <= 0) {
        throw std::runtime_error(path.string() + ": missing id or length");
    }
    validateSprites();
    return def;
}

const ShipDefinition* ShipCatalog::findById(const std::string& id) const {
    const auto it = index_.find(id);
    if (it == index_.end()) {
        return nullptr;
    }
    return &definitions_[it->second];
}
