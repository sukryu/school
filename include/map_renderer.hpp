#pragma once

#include "renderer.hpp"
#include "map.hpp"
#include "terrain_manager.hpp"

class MapRenderer {
public:
    MapRenderer(int x, int y, int width, int height);
    void draw(Renderer& renderer, const Map& map);

private:
    int x, y;
    int width, height;
};