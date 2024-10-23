#include "cursor.hpp"

Cursor::Cursor(Position initial_position)
    : current(initial_position), previous(initial_position) {}

void Cursor::move(Direction dir) {
    Position new_pos = ::move(current, dir);

    // 실제 맵이 그려지는 영역 기준으로 이동 범위 계산
    // 맵은 테두리를 제외한 영역에만 그려지므로
    // 상단과 좌측은 1부터, 하단과 우측은 -2를 해서 테두리 이전까지
    const int MIN_ROW = 0;                  // 맵 시작 위치
    const int MAX_ROW = MAP_HEIGHT - 3;     // 맵 끝 위치 (-3: 테두리와 여유 공간)
    const int MIN_COLUMN = 0;               // 맵 시작 위치
    const int MAX_COLUMN = MAP_WIDTH - 3;   // 맵 끝 위치 (-3: 테두리와 여유 공간)

    // 범위 검사
    if (new_pos.row >= MIN_ROW && new_pos.row <= MAX_ROW &&
        new_pos.column >= MIN_COLUMN && new_pos.column <= MAX_COLUMN) {
        previous = current;
        current = new_pos;
    }
}

Position Cursor::get_current_position() const {
    return current;
}

Position Cursor::get_previous_position() const {
    return previous;
}