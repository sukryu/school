#pragma once

#include "renderer.hpp"
#include "resource_bar.hpp"
#include "message_window.hpp"
#include "map_renderer.hpp"
#include "command_window.hpp"
#include "status_window.hpp"
#include "cursor.hpp"
#include "common.hpp"

class Display {
public:
    Display(int map_width, int map_height, int status_width = 20);
    void update(const Resource& resource, const Map& map, const Cursor& cursor);
    void add_system_message(const std::string& message);
    void update_status(const std::string& status);
    void update_commands(const std::vector<std::string>& commands);

private:
    Renderer renderer;
    ResourceBar resource_bar;
    MessageWindow message_window;
    CommandWindow command_window;
    StatusWindow status_window;
    MapRenderer map_renderer;
    int total_width;
    int total_height;
};