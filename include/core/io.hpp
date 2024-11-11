#pragma once
#include "../utils/types.hpp"
#include <Windows.h>
#include <conio.h>
#include <chrono>
#include <iostream>

namespace dune {
    namespace core {

        class IO {
        public:
            static void gotoxy(const types::Position& pos) {
                COORD coord = { static_cast<SHORT>(pos.column), static_cast<SHORT>(pos.row) };
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            }

            static void set_color(int color) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
            }

            static void printc(const types::Position& pos, wchar_t ch, int color) {
                set_color(color);
                gotoxy(pos);
                std::wcout << ch;
                // 색상 초기화 (선택 사항)
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), constants::color::DEFAULT);
            }

            static void clear_screen() {
                system("cls");
            }

            static types::Key get_key();
            static bool is_double_click();

        private:
            static std::chrono::steady_clock::time_point last_key_time;
            static types::Key last_key;
            static bool was_double_click;
            static constexpr std::chrono::milliseconds DOUBLE_CLICK_INTERVAL{ 100 };

            static bool check_double_click(types::Key current_key,
                std::chrono::steady_clock::time_point now);
        };
    } // namespace core
} // namespace dune
