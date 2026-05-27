#pragma once

enum class CellMark {
    Unknown,
    Miss,
    Hit,
    Sunk,
    Ship
};

enum class ShotResult {
    Miss,
    Hit,
    Sunk,
    Already
};
