#include "message_window.hpp"

MessageWindow::MessageWindow(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

void MessageWindow::add_message(const std::string& message) {
    messages.push_back(message);
    if (messages.size() > static_cast<size_t>(height)) {
        messages.pop_front();
    }
}

void MessageWindow::draw(Renderer& renderer) {
    renderer.draw_border(x, y, width, height);
    
    int message_y = y + 2;
    for (const auto& message : messages) {
        for (size_t i = 0; i < message.length() && i < static_cast<size_t>(width); ++i) {
            renderer.draw_char((x + 3) + static_cast<int>(i), message_y, message[i]);
        }
        ++message_y;
    }
}