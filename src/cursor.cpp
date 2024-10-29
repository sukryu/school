#include "cursor.hpp"

Cursor::Cursor(Position initial_position)
    : current(initial_position), previous(initial_position) {}

void Cursor::move(Direction dir) {
    previous = current;
    current = ::move(current, dir);
}

Position Cursor::get_current_position() const {
    return current;
}

Position Cursor::get_previous_position() const {
    return previous;
}

void Cursor::draw(Renderer& renderer, const Map& map) const {
    // 1. 이전 위치를 복원
    if (previous != current) {
        // 이전 위치의 화면 정보를 맵에서 가져와 백버퍼에 그립니다.
        int map_x = previous.column;
        int map_y = previous.row;

        char ch = ' ';
        int color = COLOR_DEFAULT;

        // 맵에서 해당 위치의 문자와 색상을 가져옵니다.
        const Terrain& terrain = map.get_terrain_manager().get_terrain({ map_y, map_x });
        ch = terrain.get_representation();
        color = terrain.get_color();

        // 유닛이나 건물이 있는지 확인
        if (const Unit* unit = map.get_unit_at({ map_y, map_x })) {
            ch = unit->get_representation();
            color = unit->get_color();
        }
        else if (const Building* building = map.get_building_at({ map_y, map_x })) {
            ch = building->get_representation();
            color = building->get_color();
        }

        // 백버퍼에 이전 위치의 정보를 그립니다.
        int draw_x = previous.column + 1;  // 테두리 고려
        int draw_y = previous.row + RESOURCE_HEIGHT + 1;  // 리소스 바와 테두리 고려

        renderer.draw_char(draw_x, draw_y, ch, color);
    }

    // 2. 현재 위치에 커서를 그립니다.
    int draw_x = current.column + 1;  // 테두리 고려
    int draw_y = current.row + RESOURCE_HEIGHT + 1;  // 리소스 바와 테두리 고려

    // 커서 색상으로 현재 위치에 있는 문자를 덮어씁니다.
    renderer.draw_char(draw_x, draw_y, ' ', COLOR_CURSOR);
}