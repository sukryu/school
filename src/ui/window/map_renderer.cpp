#include "../include/ui/window/map_renderer.hpp"

namespace dune {
    namespace ui {

        MapRenderer::MapRenderer(int x, int y, int width, int height)
            : BaseWindow(x, y, width, height) {}

        void MapRenderer::draw(Renderer& renderer) {
            draw_border(renderer);
        }

        void MapRenderer::draw(Renderer& renderer, const core::Map& map) {
            draw_border(renderer);
            draw_terrain(renderer, map);
            draw_buildings(renderer, map);
            draw_ground_units(renderer, map);
            draw_air_units(renderer, map);
        }

        void MapRenderer::draw_terrain(Renderer& renderer, const core::Map& map) {
            for (int row = 0; row < height - 2; ++row) {
                for (int col = 0; col < width - 2; ++col) {
                    types::Position pos{ row, col };
                    const auto& terrain = map.get_terrain_manager().get_terrain(pos);
                    char ch = terrain.get_representation();
                    int color = terrain.get_color();
                    renderer.draw_char(x + col + 1, y + row + 1, ch, color);
                }
            }
        }

        void MapRenderer::draw_buildings(Renderer& renderer, const core::Map& map) {
            for (const auto& building : map.get_building_manager().get_buildings()) {
                types::Position pos = building->get_position();
                for (int i = 0; i < building->get_height(); ++i) {
                    for (int j = 0; j < building->get_width(); ++j) {
                        int draw_x = x + pos.column + j + 1;
                        int draw_y = y + pos.row + i + 1;
                        if (draw_x >= x + 1 && draw_x < x + width - 1 &&
                            draw_y >= y + 1 && draw_y < y + height - 1) {
                            renderer.draw_char(draw_x, draw_y,
                                building->get_representation(),
                                building->get_color());
                        }
                    }
                }
            }
        }

        void MapRenderer::draw_ground_units(Renderer& renderer, const core::Map& map) {
            for (const auto& unit : map.get_unit_manager().get_units()) {
                if (!unit->is_air_unit()) {
                    types::Position pos = unit->get_position();
                    int draw_x = x + pos.column + 1;
                    int draw_y = y + pos.row + 1;
                    if (draw_x >= x + 1 && draw_x < x + width - 1 &&
                        draw_y >= y + 1 && draw_y < y + height - 1) {
                        renderer.draw_char(draw_x, draw_y,
                            unit->get_representation(),
                            unit->get_color());
                    }
                }
            }
        }

        void MapRenderer::draw_air_units(Renderer& renderer, const core::Map& map) {
            for (const auto& unit : map.get_unit_manager().get_units()) {
                if (unit->is_air_unit()) {
                    types::Position pos = unit->get_position();
                    int draw_x = x + pos.column + 1;
                    int draw_y = y + pos.row + 1;
                    if (draw_x >= x + 1 && draw_x < x + width - 1 &&
                        draw_y >= y + 1 && draw_y < y + height - 1) {
                        renderer.draw_char(draw_x, draw_y,
                            unit->get_representation(),
                            unit->get_color());
                    }
                }
            }
        }
    } // namespace ui
} // namespace dune