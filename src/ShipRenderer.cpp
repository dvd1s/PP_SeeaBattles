#include "ShipRenderer.hpp"

namespace {

void putCell(std::vector<std::string>& grid, int col, int row, char ch) {
    if (row < 0 || row >= static_cast<int>(grid.size())) {
        return;
    }
    if (col < 0 || col >= static_cast<int>(grid[row].size())) {
        return;
    }
    grid[row][col] = ch;
}

}  // namespace

void ShipRenderer::drawOntoGrid(
    std::vector<std::string>& grid,
    Coord anchor,
    Direction direction,
    const ShipDefinition& def,
    char overlayChar,
    bool validPlacement) {
    const char marker = validPlacement ? overlayChar : '?';
    const auto& sprite = direction == Direction::Horizontal ? def.spriteH : def.spriteV;

    if (direction == Direction::Horizontal) {
        if (sprite.empty()) {
            return;
        }
        const std::string& row = sprite[0];
        for (int i = 0; i < static_cast<int>(row.size()); ++i) {
            if (row[i] != ' ') {
                putCell(grid, anchor.col + i, anchor.row, marker);
            }
        }
        return;
    }

    for (int i = 0; i < static_cast<int>(sprite.size()) && i < def.length; ++i) {
        if (!sprite[i].empty() && sprite[i][0] != ' ') {
            putCell(grid, anchor.col, anchor.row + i, marker);
        }
    }
}
