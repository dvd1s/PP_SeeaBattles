#include "FleetConfig.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

FleetConfig FleetConfig::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Cannot open fleet config: " + path);
    }

    FleetConfig config;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty() || line[0] == '#') {
            continue;
        }
        const auto eq = line.find('=');
        if (eq == std::string::npos) {
            continue;
        }
        FleetEntry entry;
        entry.shipId = line.substr(0, eq);
        entry.count = std::stoi(line.substr(eq + 1));
        config.entries_.push_back(entry);
    }

    if (config.entries_.empty()) {
        throw std::runtime_error("Fleet config is empty: " + path);
    }
    return config;
}

std::vector<std::string> FleetConfig::buildPlacementQueue() const {
    std::vector<std::string> queue;
    for (const auto& entry : entries_) {
        for (int i = 0; i < entry.count; ++i) {
            queue.push_back(entry.shipId);
        }
    }
    return queue;
}
