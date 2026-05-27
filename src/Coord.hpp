#pragma once

struct Coord {
    int col = 0;
    int row = 0;

    bool operator==(const Coord& other) const {
        return col == other.col && row == other.row;
    }

    bool operator<(const Coord& other) const {
        if (row != other.row) {
            return row < other.row;
        }
        return col < other.col;
    }
};

inline constexpr int kBoardSize = 10;

inline bool inBounds(Coord c) {
    return c.col >= 0 && c.col < kBoardSize && c.row >= 0 && c.row < kBoardSize;
}
