#pragma once
#include "base_window.hpp"
#include "../../core/map.hpp"

namespace dune {
    namespace ui {

        class MapRenderer : public BaseWindow {
        public:
            MapRenderer(int x, int y, int width, int height);
            void draw(Renderer& renderer) override;
            void draw(Renderer& renderer, const core::Map& map);

        private:
            void draw_terrain(Renderer& renderer, const core::Map& map);
            void draw_buildings(Renderer& renderer, const core::Map& map);
            void draw_ground_units(Renderer& renderer, const core::Map& map);
            void draw_air_units(Renderer& renderer, const core::Map& map);
        };
    } // namespace ui
} // namespace dune