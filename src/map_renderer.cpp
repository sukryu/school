#include "map_renderer.hpp"

MapRenderer::MapRenderer(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

#include "map_renderer.hpp"

void MapRenderer::draw(Renderer& renderer, const Map& map) {
    // 테두리 그리기
    renderer.draw_border(x, y, width, height);

    // 지형, 건물, 유닛 그리기 (테두리 안쪽에 그리기 위해 좌표 조정)
    for (int row = 0; row < height - 2; ++row) {
        for (int col = 0; col < width - 2; ++col) {
            Position pos{ row, col };
            const Terrain& terrain = map.get_terrain_manager().get_terrain(pos);
            char ch = terrain.get_representation();
            int color = terrain.get_color();
            renderer.draw_char(x + col + 1, y + row + 1, ch, color);
        }
    }

    // 건물과 유닛도 마찬가지로 테두리 안쪽에 그립니다.
    // 건물 그리기
    for (const auto& building : map.get_building_manager().get_buildings()) {
        Position pos = building->get_position();
        for (int i = 0; i < building->get_height(); ++i) {
            for (int j = 0; j < building->get_width(); ++j) {
                int draw_x = x + pos.column + j + 1;
                int draw_y = y + pos.row + i + 1;
                if (draw_x >= x + 1 && draw_x < x + width - 1 &&
                    draw_y >= y + 1 && draw_y < y + height - 1) {
                    char ch = building->get_representation();
                    int color = building->get_color();
                    renderer.draw_char(draw_x, draw_y, ch, color);
                }
            }
        }
    }

    // 유닛 그리기
    for (const auto& unit : map.get_unit_manager().get_units()) {
        Position pos = unit->get_position();
        int draw_x = x + pos.column + 1;
        int draw_y = y + pos.row + 1;
        if (draw_x >= x + 1 && draw_x < x + width - 1 &&
            draw_y >= y + 1 && draw_y < y + height - 1) {
            char ch = unit->get_representation();
            int color = unit->get_color();
            renderer.draw_char(draw_x, draw_y, ch, color);
        }
    }
}