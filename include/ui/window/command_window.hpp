#pragma once
#include "base_window.hpp"
#include <vector>
#include <string>

namespace dune {
    namespace ui {

        /**
         * @brief 게임 명령어를 표시하는 윈도우 클래스입니다.
         */
        class CommandWindow : public BaseWindow {
        public:
            /**
             * @brief CommandWindow 클래스의 생성자입니다.
             * @param x 윈도우의 x 좌표.
             * @param y 윈도우의 y 좌표.
             * @param width 윈도우의 너비.
             * @param height 윈도우의 높이.
             */
            CommandWindow(int x, int y, int width, int height);

            /**
             * @brief 명령어 목록을 업데이트합니다.
             * @param commands 업데이트할 명령어 리스트.
             */
            void updateCommands(const std::vector<std::wstring>& commands);

            /**
             * @brief 윈도우를 그립니다.
             * @param renderer 렌더러 객체.
             */
            void draw(Renderer& renderer) override;

        private:
            std::vector<std::wstring> commands_;
        };
    } // namespace ui
} // namespace dune
