#pragma once

#include <string>
#include <vector>

struct FleetEntry {
    std::string shipId;
    int count = 0;
};

class FleetConfig {
public:
    static FleetConfig loadFromFile(const std::string& path);
    const std::vector<FleetEntry>& entries() const { return entries_; }

    std::vector<std::string> buildPlacementQueue() const;

private:
    std::vector<FleetEntry> entries_;
};
