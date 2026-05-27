#include "ConsoleIO.hpp"

#include <conio.h>
#include <iostream>
#include <string>
#include <windows.h>

void ConsoleIO::clearScreen() {
    system("cls");
}

void ConsoleIO::hideCursor() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info{};
    GetConsoleCursorInfo(out, &info);
    info.bVisible = FALSE;
    SetConsoleCursorInfo(out, &info);
}

void ConsoleIO::showCursor() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info{};
    GetConsoleCursorInfo(out, &info);
    info.bVisible = TRUE;
    SetConsoleCursorInfo(out, &info);
}

void ConsoleIO::setCursorPosition(int x, int y) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos{};
    pos.X = static_cast<SHORT>(x);
    pos.Y = static_cast<SHORT>(y);
    SetConsoleCursorPosition(out, pos);
}

void ConsoleIO::writeAt(int x, int y, char ch) {
    setCursorPosition(x, y);
    std::cout << ch;
}

KeyAction ConsoleIO::readKey() {
    const int ch = _getch();
    if (ch == 0 || ch == 224) {
        const int code = _getch();
        switch (code) {
        case 72:
            return KeyAction::Up;
        case 80:
            return KeyAction::Down;
        case 75:
            return KeyAction::Left;
        case 77:
            return KeyAction::Right;
        default:
            return KeyAction::None;
        }
    }

    switch (ch) {
    case 13:
        return KeyAction::Enter;
    case 27:
        return KeyAction::Escape;
    case ' ':
        return KeyAction::Rotate;
    case 'r':
    case 'R':
        return KeyAction::RandomFleet;
    case 'q':
    case 'Q':
        return KeyAction::Quit;
    default:
        return KeyAction::None;
    }
}

std::string ConsoleIO::readLine(const char* prompt) {
    showCursor();
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    hideCursor();
    return line;
}

void ConsoleIO::waitForAnyKey(const char* message) {
    std::cout << message;
    _getch();
}

void ConsoleIO::pauseBetweenPlayers(const std::string& playerName) {
    clearScreen();
    std::cout << playerName << ", press any key when ready...\n";
    _getch();
}
