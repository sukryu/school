#include "status_window.hpp"

StatusWindow::StatusWindow(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

void StatusWindow::update_status(const std::string& status) {
    status_text = status;
}

void StatusWindow::draw(Renderer& renderer) {
    renderer.draw_border(x, y, width, height);
    for (size_t i = 0; i < status_text.length() && i < static_cast<size_t>(width - 2); ++i) {
        renderer.draw_char(x + 1 + static_cast<int>(i), y + 1, status_text[i]);
    }
}