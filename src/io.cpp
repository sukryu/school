#include "io.hpp"

void IO::gotoxy(const Position& pos) {
    COORD coord = { static_cast<SHORT>(pos.column), static_cast<SHORT>(pos.row) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void IO::set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void IO::printc(const Position& pos, char ch, int color) {
    if (color >= 0) {
        set_color(color);
    }
    gotoxy(pos);
    std::cout << ch;
}

Key IO::get_key() {
    if (!_kbhit()) {
        return Key::None;
    }

    int byte = _getch();
    switch (byte) {
    case 'q': return Key::Quit;
    case 224:
        byte = _getch();
        switch (byte) {
        case 72: return Key::Up;
        case 75: return Key::Left;
        case 77: return Key::Right;
        case 80: return Key::Down;
        default: return Key::Undefined;
        }
    default: return Key::Undefined;
    }
}
