#pragma once

enum class Direction {
    Horizontal,
    Vertical
};

inline Direction flip(Direction d) {
    return d == Direction::Horizontal ? Direction::Vertical : Direction::Horizontal;
}
