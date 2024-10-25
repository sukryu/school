#include "io.hpp"

std::chrono::steady_clock::time_point IO::last_key_time;
Key IO::last_key = Key::None;
bool IO::was_double_click = false;

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
        was_double_click = false;  // 키 입력이 없으면 더블클릭 상태 초기화
        return Key::None;
    }

    int byte = _getch();
    Key current_key;

    switch (byte) {
    case 'q':
        current_key = Key::Quit;
        break;
    case ' ':
        current_key = Key::Space;
        break;
    case 27:
        current_key = Key::Esc;
        break;
    case 224: {
        byte = _getch();
        switch (byte) {
        case 72:
            current_key = Key::Up;
            break;
        case 75:
            current_key = Key::Left;
            break;
        case 77:
            current_key = Key::Right;
            break;
        case 80:
            current_key = Key::Down;
            break;
        default:
            current_key = Key::Undefined;
            break;
        }
        break;
    }
    default:
        current_key = Key::Undefined;
        break;
    }

    auto now = std::chrono::steady_clock::now();
    is_double_click(current_key, now);

    if (current_key != Key::None) {
        last_key = current_key;
        last_key_time = now;
    }

    return current_key;
}

bool IO::is_double_click(Key current_key, std::chrono::steady_clock::time_point now) {
    if (current_key != last_key) {
        return false;
    }

    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_key_time);
    bool is_double = diff < DOUBLE_CLICK_INTERVAL;

    if (is_double) {
        was_double_click = true;
        // 마지막 키 입력 시간을 현재보다 충분히 이전으로 설정
        last_key_time = now - std::chrono::milliseconds{ 500 };
    }

    return was_double_click;
}

bool IO::is_double_click() {
    return was_double_click;
}