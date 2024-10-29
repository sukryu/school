#pragma once

#include "common.hpp"
#include <conio.h>

class IO {
public:
    static void gotoxy(const Position& pos);
    static void set_color(int color);
    static void printc(const Position& pos, char ch, int color);
    static Key get_key();
    static bool is_double_click();

private:
    static std::chrono::steady_clock::time_point last_key_time;
    static Key last_key;
    static bool was_double_click;
    static constexpr std::chrono::milliseconds DOUBLE_CLICK_INTERVAL{ 100 }; // 더블클릭 간격
    static bool is_double_click(Key current_key, std::chrono::steady_clock::time_point now);
};