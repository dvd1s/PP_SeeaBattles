#pragma once

#include <string>
#include <vector>

struct ShipDefinition {
    std::string id;
    std::string name;
    int length = 0;
    char bodyChar = '#';
    char hitChar = 'X';
    char sunkChar = '%';
    std::vector<std::string> spriteH;
    std::vector<std::string> spriteV;
};
