#pragma once

#include "common.hpp"
#include <vector>

class Renderer {
public:
    Renderer(int width, int height);
    void clear();
    void draw_char(int x, int y, char ch, int color = COLOR_DEFAULT);
    void render();
    void draw_border(int x, int y, int width, int height);

    char get_char_at(int x, int y) const;
    int get_color_at(int x, int y) const;

private:
    int width;
    int height;
    std::vector<std::vector<char>> back_buffer;
    std::vector<std::vector<char>> front_buffer;
    std::vector<std::vector<int>> color_buffer;
    std::vector<std::vector<int>> front_color_buffer;
};