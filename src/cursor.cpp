#include "cursor.hpp"

Cursor::Cursor(Position initial_position)
    : current(initial_position), previous(initial_position) {}

void Cursor::move(Direction dir) {
    Position new_pos = ::move(current, dir);

    // 맵의 실제 이동 가능 영역 체크
    if (new_pos.row >= 0 && new_pos.row < MAP_HEIGHT - 3 &&
        new_pos.column >= 0 && new_pos.column < MAP_WIDTH - 3) {
        previous = current;  // 현재 위치를 이전 위치로 저장
        current = new_pos;   // 새 위치로 이동
    }
}

Position Cursor::get_current_position() const {
    return current;
}

Position Cursor::get_previous_position() const {
    return previous;
}