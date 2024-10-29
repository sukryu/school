#pragma once

#include "renderer.hpp"
#include <string>

class StatusWindow {
public:
    StatusWindow(int x, int y, int width, int height);
    void update_status(const std::string& status);
    void draw(Renderer& renderer);

private:
    int x, y;
    int width, height;
    std::string status_text;
};