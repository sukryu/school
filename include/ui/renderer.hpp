#pragma once
#include "../utils/types.hpp"
#include "../utils/constants.hpp"
#include <vector>
#include <string>

namespace dune {
    namespace ui {

        /**
         * @brief 화면에 그리기를 수행하는 렌더러 클래스입니다.
         */
        class Renderer {
        public:
            /**
             * @brief Renderer 클래스의 생성자입니다.
             * @param width 렌더러의 너비.
             * @param height 렌더러의 높이.
             */
            Renderer(int width, int height);

            /**
             * @brief 백 버퍼를 초기화합니다.
             */
            void clear();

            /**
             * @brief 특정 위치에 문자를 그립니다.
             * @param x x 좌표.
             * @param y y 좌표.
             * @param ch 그릴 문자.
             * @param color 색상 코드.
             */
            void drawChar(int x, int y, wchar_t ch, int color = constants::color::DEFAULT);

            /**
             * @brief 특정 위치에 문자열을 그립니다.
             * @param x x 좌표.
             * @param y y 좌표.
             * @param str 그릴 문자열.
             * @param color 색상 코드.
             */
            void drawString(int x, int y, const std::wstring& str, int color = constants::color::DEFAULT);

            /**
             * @brief 테두리를 그립니다.
             * @param x x 좌표.
             * @param y y 좌표.
             * @param width 너비.
             * @param height 높이.
             */
            void drawBorder(int x, int y, int width, int height);

            /**
             * @brief 버퍼의 변경된 부분만 화면에 출력합니다.
             */
            void render();

            // 상태 확인
            wchar_t getCharAt(int x, int y) const;
            int getColorAt(int x, int y) const;
            bool isValidPosition(int x, int y) const;

            // 접근자
            int getWidth() const { return width_; }
            int getHeight() const { return height_; }

        private:
            int width_;
            int height_;
            std::vector<std::vector<wchar_t>> backBuffer_;
            std::vector<std::vector<wchar_t>> frontBuffer_;
            std::vector<std::vector<int>> colorBuffer_;
            std::vector<std::vector<int>> frontColorBuffer_;

            void initBuffers();
            bool needsUpdate(int x, int y) const;
            void updateFrontBuffer(int x, int y);
        };
    } // namespace ui
} // namespace dune
