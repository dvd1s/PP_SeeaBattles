#include "Ship.hpp"

Ship::Ship(const ShipDefinition& def, Coord anchor, Direction dir)
    : definitionId_(def.id),
      name_(def.name),
      length_(def.length),
      anchor_(anchor),
      direction_(dir),
      bodyChar_(def.bodyChar),
      hitChar_(def.hitChar),
      sunkChar_(def.sunkChar) {}

std::vector<Coord> Ship::cells() const {
    std::vector<Coord> result;
    result.reserve(static_cast<size_t>(length_));
    for (int i = 0; i < length_; ++i) {
        if (direction_ == Direction::Horizontal) {
            result.push_back({anchor_.col + i, anchor_.row});
        } else {
            result.push_back({anchor_.col, anchor_.row + i});
        }
    }
    return result;
}

bool Ship::contains(Coord c) const {
    for (const auto cell : cells()) {
        if (cell == c) {
            return true;
        }
    }
    return false;
}

void Ship::registerHit(Coord c) {
    if (contains(c)) {
        hits_.insert(c);
    }
}

bool Ship::isHit(Coord c) const {
    return hits_.count(c) > 0;
}

bool Ship::isSunk() const {
    return static_cast<int>(hits_.size()) >= length_;
}
