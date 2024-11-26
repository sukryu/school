#pragma once
#include "../renderer.hpp"
#include <string>

namespace dune {
    namespace ui {

        /**
         * @brief 모든 윈도우 클래스의 기본 클래스입니다.
         */
        class BaseWindow {
        public:
            /**
             * @brief BaseWindow 클래스의 생성자입니다.
             * @param x 윈도우의 x 좌표.
             * @param y 윈도우의 y 좌표.
             * @param width 윈도우의 너비.
             * @param height 윈도우의 높이.
             */
            BaseWindow(int x, int y, int width, int height)
                : x_(x), y_(y), width_(width), height_(height) {}

            virtual ~BaseWindow() = default;

            /**
             * @brief 윈도우를 그리는 순수 가상 함수입니다.
             * @param renderer 렌더러 객체.
             */
            virtual void draw(Renderer& renderer) = 0;

            // 접근자 메서드
            int getX() const { return x_; }
            int getY() const { return y_; }
            int getWidth() const { return width_; }
            int getHeight() const { return height_; }

        protected:
            int x_;
            int y_;
            int width_;
            int height_;

            /**
             * @brief 윈도우의 테두리를 그립니다.
             * @param renderer 렌더러 객체.
             */
            void drawBorder(Renderer& renderer) const;

            /**
             * @brief 윈도우 내 상대 위치에 텍스트를 출력합니다.
             * @param renderer 렌더러 객체.
             * @param relX 상대 x 좌표.
             * @param relY 상대 y 좌표.
             * @param text 출력할 텍스트.
             * @param color 텍스트 색상.
             */
            void drawText(Renderer& renderer, int relX, int relY,
                const std::wstring& text, int color = constants::color::DEFAULT) const;
        };
    } // namespace ui
} // namespace dune
