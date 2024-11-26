#pragma once
#include "base_window.hpp"
#include "../../core/map.hpp"

namespace dune {
    namespace ui {

        /**
         * @brief 게임 맵과 관련된 요소를 렌더링하는 클래스입니다.
         */
        class MapRenderer : public BaseWindow {
        public:
            /**
             * @brief MapRenderer 클래스의 생성자입니다.
             * @param x 윈도우의 x 좌표.
             * @param y 윈도우의 y 좌표.
             * @param width 윈도우의 너비.
             * @param height 윈도우의 높이.
             */
            MapRenderer(int x, int y, int width, int height);

            /**
             * @brief 윈도우를 그립니다.
             * @param renderer 렌더러 객체.
             */
            void draw(Renderer& renderer) override;

            /**
             * @brief 맵을 렌더링합니다.
             * @param renderer 렌더러 객체.
             * @param map 렌더링할 맵 객체.
             */
            void draw(Renderer& renderer, const core::Map& map);

        private:
            void drawTerrain(Renderer& renderer, const core::Map& map);
            void drawBuildings(Renderer& renderer, const core::Map& map);
            void drawGroundUnits(Renderer& renderer, const core::Map& map);
        };
    } // namespace ui
} // namespace dune
