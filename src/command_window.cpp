#include "command_window.hpp"

CommandWindow::CommandWindow(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

void CommandWindow::update_commands(const std::vector<std::string>& commands_list) {
    commands = commands_list;
}

void CommandWindow::draw(Renderer& renderer) {
    renderer.draw_border(x, y, width, height);
    int command_y = y + 2;
    for (const auto& command : commands) {
        for (size_t i = 0; i < command.length() && i < static_cast<size_t>(width); ++i) {
            renderer.draw_char((x + 3) + static_cast<int>(i), command_y, command[i]);
        }
        ++command_y;
    }
}