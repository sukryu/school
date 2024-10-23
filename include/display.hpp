#pragma once
#include "common.hpp"
#include "map.hpp"
#include "cursor.hpp"
#include <vector>
#include <string>

class Display {
public:
    Display(int map_width, int map_height, int status_width = 20);
    void update(const Resource& resource, const Map& map, const Cursor& cursor);
    void add_system_message(const std::string& message);
    void add_command_message(const std::string& message);

private:
    static constexpr int RESOURCE_HEIGHT = 1;
    static constexpr int SYSTEM_MESSAGE_HEIGHT = 10;
    static constexpr int COMMAND_HEIGHT = 10;

    void display_resource(const Resource& resource);
    void display_map(const Map& map);
    void display_cursor(const Cursor& cursor);
    void display_status_window();
    void display_system_messages();
    void display_command_window();
    void draw_border(int x, int y, int width, int height);
    void update_screen();

    int map_width;
    int map_height;
    int status_width;
    int total_width;
    int total_height;
    std::vector<std::vector<char>> backbuf;
    std::vector<std::vector<char>> frontbuf;
    std::vector<std::vector<int>> colorbuf;  // 색상 버퍼 추가
    std::vector<std::string> system_messages;
    std::vector<std::string> command_messages;
};