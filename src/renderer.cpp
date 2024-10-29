#include "renderer.hpp"
#include "io.hpp"

Renderer::Renderer(int width, int height)
    : width(width), height(height),
    back_buffer(height, std::vector<char>(width, ' ')),
    front_buffer(height, std::vector<char>(width, ' ')),
    color_buffer(height, std::vector<int>(width, COLOR_DEFAULT)),
    front_color_buffer(height, std::vector<int>(width, COLOR_DEFAULT)) {}

void Renderer::clear() {
    for (int y = 0; y < height; ++y) {
        std::fill(back_buffer[y].begin(), back_buffer[y].end(), ' ');
        std::fill(color_buffer[y].begin(), color_buffer[y].end(), COLOR_DEFAULT);
    }
}

void Renderer::draw_char(int x, int y, char ch, int color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        back_buffer[y][x] = ch;
        color_buffer[y][x] = color;
    }
}

void Renderer::render() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (back_buffer[y][x] != front_buffer[y][x] ||
                color_buffer[y][x] != front_color_buffer[y][x]) {
                IO::printc({ y, x }, back_buffer[y][x], color_buffer[y][x]);
                front_buffer[y][x] = back_buffer[y][x];
                front_color_buffer[y][x] = color_buffer[y][x];
            }
        }
    }
}

void Renderer::draw_border(int x, int y, int w, int h) {
    // 상단과 하단 경계선 그리기
    for (int i = x; i < x + w; ++i) {
        draw_char(i, y, '#');
        draw_char(i, y + h - 1, '#');
    }
    // 좌우 경계선 그리기
    for (int i = y; i < y + h; ++i) {
        draw_char(x, i, '#');
        draw_char(x + w - 1, i, '#');
    }
}

char Renderer::get_char_at(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return back_buffer[y][x];
    }
    return ' ';
}

int Renderer::get_color_at(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return color_buffer[y][x];
    }
    return COLOR_DEFAULT;
}