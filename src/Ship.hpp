#pragma once

#include "Coord.hpp"
#include "Direction.hpp"
#include "ShipDefinition.hpp"

#include <set>
#include <string>
#include <vector>

class Ship {
public:
    Ship() = default;
    Ship(const ShipDefinition& def, Coord anchor, Direction dir);

    const std::string& definitionId() const { return definitionId_; }
    const std::string& name() const { return name_; }
    int length() const { return length_; }
    Coord anchor() const { return anchor_; }
    Direction direction() const { return direction_; }
    char bodyChar() const { return bodyChar_; }
    char hitChar() const { return hitChar_; }
    char sunkChar() const { return sunkChar_; }

    std::vector<Coord> cells() const;
    bool contains(Coord c) const;
    void registerHit(Coord c);
    bool isHit(Coord c) const;
    bool isSunk() const;

private:
    std::string definitionId_;
    std::string name_;
    int length_ = 0;
    Coord anchor_{};
    Direction direction_ = Direction::Horizontal;
    char bodyChar_ = '#';
    char hitChar_ = 'X';
    char sunkChar_ = '%';
    std::set<Coord> hits_;
};
