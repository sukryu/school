#pragma once

#include "common.hpp"
#include <Windows.h>
#include <conio.h>

class IO {
public:
    static void gotoxy(const Position& pos);
    static void set_color(int color);
    static void printc(const Position& pos, char ch, int color);
    static Key get_key();
};