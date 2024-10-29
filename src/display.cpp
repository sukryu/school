#include "display.hpp"

Display::Display(int map_width, int map_height, int status_width)
    : renderer(map_width + status_width, RESOURCE_HEIGHT + map_height + SYSTEM_MESSAGE_HEIGHT),
    resource_bar(map_width + status_width),
    message_window(0, RESOURCE_HEIGHT + map_height, map_width, SYSTEM_MESSAGE_HEIGHT),
    command_window(map_width, RESOURCE_HEIGHT + map_height, status_width, SYSTEM_MESSAGE_HEIGHT),
    status_window(map_width, RESOURCE_HEIGHT, status_width, map_height),
    map_renderer(0, RESOURCE_HEIGHT, map_width, map_height),
    total_width(map_width + status_width),
    total_height(RESOURCE_HEIGHT + map_height + SYSTEM_MESSAGE_HEIGHT) {}

void Display::update(const Resource& resource, const Map& map, const Cursor& cursor) {
    renderer.clear();

    // 리소스 바 업데이트 및 그리기
    resource_bar.update(resource);
    resource_bar.draw(renderer);

    // 맵 그리기
    map_renderer.draw(renderer, map);

    // 상태 창 및 명령 창 그리기
    status_window.draw(renderer);
    command_window.draw(renderer);

    // 시스템 메시지 창 그리기
    message_window.draw(renderer);

    // 커서 그리기 (항상 맨 마지막에 그리도록 순서 변경)
    cursor.draw(renderer, map);

    // 화면 렌더링
    renderer.render();
}

void Display::add_system_message(const std::string& message) {
    message_window.add_message(message);
}

void Display::update_status(const std::string& status) {
    status_window.update_status(status);
}

void Display::update_commands(const std::vector<std::string>& commands) {
    command_window.update_commands(commands);
}