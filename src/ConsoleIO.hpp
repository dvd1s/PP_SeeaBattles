#pragma once

#include "Coord.hpp"

#include <string>

enum class KeyAction {
    None,
    Up,
    Down,
    Left,
    Right,
    Enter,
    Rotate,
    RandomFleet,  // R — place entire fleet randomly
    Escape,
    Quit
};

class ConsoleIO {
public:
    static void clearScreen();
    static void hideCursor();
    static void showCursor();
    static void setCursorPosition(int x, int y);
    static void writeAt(int x, int y, char ch);

    static KeyAction readKey();
    static std::string readLine(const char* prompt);
    static void waitForAnyKey(const char* message);
    static void pauseBetweenPlayers(const std::string& playerName);
};
