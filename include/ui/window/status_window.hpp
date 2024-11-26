#pragma once
#include "base_window.hpp"
#include <string>

namespace dune {
    namespace ui {

        /**
         * @brief 현재 선택된 객체의 상태를 표시하는 윈도우 클래스입니다.
         */
        class StatusWindow : public BaseWindow {
        public:
            /**
             * @brief StatusWindow 클래스의 생성자입니다.
             * @param x 윈도우의 x 좌표.
             * @param y 윈도우의 y 좌표.
             * @param width 윈도우의 너비.
             * @param height 윈도우의 높이.
             */
            StatusWindow(int x, int y, int width, int height);

            /**
             * @brief 상태 메시지를 업데이트합니다.
             * @param status 업데이트할 상태 메시지.
             */
            void updateStatus(const std::wstring& status);

            /**
             * @brief 윈도우를 그립니다.
             * @param renderer 렌더러 객체.
             */
            void draw(Renderer& renderer) override;

            /**
             * @brief 현재 상태 메시지를 반환합니다.
             * @return const std::wstring& 상태 메시지.
             */
            const std::wstring& getCurrentStatus() const { return statusText_; }

        private:
            std::wstring statusText_;
        };
    } // namespace ui
} // namespace dune
