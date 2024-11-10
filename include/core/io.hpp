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
            static void init() {
                // 콘솔 초기화 (커서 숨기기 등)
                HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_CURSOR_INFO cursorInfo;
                GetConsoleCursorInfo(console, &cursorInfo);
                cursorInfo.bVisible = false;
                SetConsoleCursorInfo(console, &cursorInfo);
            }

            static void cleanup() {
                // 프로그램 종료 시 정리
                set_color(constants::color::DEFAULT);
                system("cls");
            }

            static void gotoxy(const types::Position& pos) {
                COORD coord = {
                    static_cast<SHORT>(pos.column),
                    static_cast<SHORT>(pos.row)
                };
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            }

            static void set_color(int color) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
            }

            static void printc(const types::Position& pos, char ch, int color) {
                set_color(color);
                gotoxy(pos);
                std::cout << ch;
            }

            static void clear_screen() {
                system("cls");
            }

            static types::Key get_key() {
                if (!_kbhit()) {
                    was_double_click = false;
                    return types::Key::None;
                }

                int byte = _getch();
                return parse_key_input(byte);
            }

            static bool is_double_click() {
                return was_double_click;
            }

        private:
            static std::chrono::steady_clock::time_point last_key_time;
            static types::Key last_key;
            static bool was_double_click;

            static types::Key parse_key_input(int byte) {
                types::Key current_key;

                switch (byte) {
                case 'q': case 'Q':
                    current_key = types::Key::Quit;
                    break;
                case ' ':
                    current_key = types::Key::Space;
                    break;
                case 27:  // ESC
                    current_key = types::Key::Esc;
                    break;
                case 'b': case 'B':
                    current_key = types::Key::Build;
                    break;
                case 't': case 'T':
                    current_key = types::Key::Train;
                    break;
                case 'h': case 'H':
                    current_key = types::Key::Harvest;
                    break;
                case 224: { // 방향키
                    byte = _getch();
                    switch (byte) {
                    case 72: current_key = types::Key::Up; break;
                    case 75: current_key = types::Key::Left; break;
                    case 77: current_key = types::Key::Right; break;
                    case 80: current_key = types::Key::Down; break;
                    default: current_key = types::Key::Undefined; break;
                    }
                    break;
                }
                default:
                    current_key = types::Key::Undefined;
                    break;
                }

                auto now = std::chrono::steady_clock::now();
                check_double_click(current_key, now);

                if (current_key != types::Key::None) {
                    last_key = current_key;
                    last_key_time = now;
                }

                return current_key;
            }

            static void check_double_click(types::Key current_key,
                std::chrono::steady_clock::time_point now) {
                if (current_key != last_key) {
                    was_double_click = false;
                    return;
                }

                auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now - last_key_time);

                was_double_click = (diff < std::chrono::milliseconds(
                    constants::DOUBLE_CLICK_INTERVAL));

                if (was_double_click) {
                    // 더블클릭 이후 바로 다음 클릭이 더블클릭으로 인식되는 것을 방지
                    last_key_time = now - std::chrono::milliseconds(500);
                }
            }
        };

        // Static 멤버 변수 정의
        std::chrono::steady_clock::time_point IO::last_key_time;
        types::Key IO::last_key = types::Key::None;
        bool IO::was_double_click = false;
	} // namespace core
} // namespace dune