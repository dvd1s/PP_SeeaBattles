#include "BoardRenderer.hpp"

#include "ShipRenderer.hpp"

#include <iomanip>
#include <sstream>

namespace {

std::vector<std::string> emptyGrid() {
    return std::vector<std::string>(kBoardSize, std::string(kBoardSize, '.'));
}

}  // namespace

char BoardRenderer::columnLabel(int col) {
    return static_cast<char>('A' + col);
}

std::vector<std::string> BoardRenderer::buildDisplayGrid(
    const Board& board,
    bool showFleet,
    const ShipCatalog* catalog,
    const std::optional<ShipDefinition>& previewDef,
    Coord previewAnchor,
    Direction previewDir,
    bool previewValid) {
    auto grid = emptyGrid();
    for (int row = 0; row < kBoardSize; ++row) {
        for (int col = 0; col < kBoardSize; ++col) {
            grid[row][col] = board.displayCharAt({col, row}, showFleet);
        }
    }

    if (previewDef.has_value()) {
        ShipRenderer::drawOntoGrid(grid, previewAnchor, previewDir, *previewDef, '*', previewValid);
    }

    return grid;
}

void BoardRenderer::printGrid(const std::vector<std::string>& grid, Coord cursor, bool showCursor) {
    std::cout << "   ";
    for (int col = 0; col < kBoardSize; ++col) {
        std::cout << ' ' << columnLabel(col);
    }
    std::cout << '\n';

    for (int row = 0; row < kBoardSize; ++row) {
        std::cout << std::setw(2) << (row + 1) << ' ';
        for (int col = 0; col < kBoardSize; ++col) {
            char ch = grid[row][col];
            if (showCursor && cursor.col == col && cursor.row == row) {
                ch = '>';
            }
            std::cout << ' ' << ch;
        }
        std::cout << '\n';
    }
}

void BoardRenderer::printTargetingBoard(const Board& board, Coord cursor) {
    const auto grid = buildDisplayGrid(board, false, nullptr, std::nullopt, {}, Direction::Horizontal, true);
    printGrid(grid, cursor, true);
}

void BoardRenderer::printOwnBoard(
    const Board& board,
    const ShipCatalog& catalog,
    Coord cursor,
    const std::optional<ShipDefinition>& previewDef,
    Direction previewDir,
    bool previewValid) {
    (void)catalog;
    const auto grid = buildDisplayGrid(
        board, true, &catalog, previewDef, cursor, previewDir, previewValid);
    printGrid(grid, cursor, previewDef.has_value());
}
