#pragma once

#include "renderer.hpp"
#include <vector>
#include <string>

class CommandWindow {
public:
    CommandWindow(int x, int y, int width, int height);
    void update_commands(const std::vector<std::string>& commands);
    void draw(Renderer& renderer);

private:
    int x, y;
    int width, height;
    std::vector<std::string> commands;
};