#pragma once
#include "../utils/types.hpp"
#include <Windows.h>
#include <conio.h>
#include <chrono>
#include <iostream>

namespace dune {
    namespace core {

        /**
         * @brief 콘솔 입출력 및 사용자 입력 처리를 담당하는 클래스입니다.
         */
        class IO {
        public:
            /**
             * @brief 콘솔 커서를 지정한 위치로 이동합니다.
             * @param position 이동할 위치.
             */
            static void gotoxy(const types::Position& position) {
                COORD coord = { static_cast<SHORT>(position.column), static_cast<SHORT>(position.row) };
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            }

            /**
             * @brief 콘솔 텍스트의 색상을 설정합니다.
             * @param color 색상 코드.
             */
            static void setColor(int color) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
            }

            /**
             * @brief 지정한 위치에 문자를 특정 색상으로 출력합니다.
             * @param position 출력할 위치.
             * @param character 출력할 문자.
             * @param color 색상 코드.
             */
            static void printChar(const types::Position& position, wchar_t character, int color) {
                setColor(color);
                gotoxy(position);
                std::wcout << character;
                // 색상 초기화 (선택 사항)
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), constants::color::DEFAULT);
            }

            /**
             * @brief 콘솔 화면을 지웁니다.
             */
            static void clearScreen() {
                system("cls");
            }

            /**
             * @brief 사용자로부터 키 입력을 받아 처리합니다.
             * @return types::Key 입력된 키에 해당하는 열거형 값.
             */
            static types::Key getKey();

            /**
             * @brief 더블클릭 여부를 확인합니다.
             * @return true 더블클릭이면 true.
             * @return false 아니면 false.
             */
            static bool isDoubleClick();

        private:
            static std::chrono::steady_clock::time_point lastKeyTime_;
            static types::Key lastKey_;
            static bool wasDoubleClick_;
            static constexpr std::chrono::milliseconds DOUBLE_CLICK_INTERVAL_{ 100 };

            /**
             * @brief 더블클릭을 체크합니다.
             * @param currentKey 현재 입력된 키.
             * @param now 현재 시간.
             * @return true 더블클릭이면 true.
             * @return false 아니면 false.
             */
            static bool checkDoubleClick(types::Key currentKey,
                std::chrono::steady_clock::time_point now);
        };

    } // namespace core
} // namespace dune
