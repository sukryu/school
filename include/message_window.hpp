#pragma once

#include "renderer.hpp"
#include <string>
#include <deque>

class MessageWindow {
public:
    MessageWindow(int x, int y, int width, int height);
    void add_message(const std::string& message);
    void draw(Renderer& renderer);

private:
    int x, y;
    int width, height;
    std::deque<std::string> messages;
};