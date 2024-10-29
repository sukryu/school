#pragma once

#include "renderer.hpp"
#include "common.hpp"

class ResourceBar {
public:
    ResourceBar(int width);
    void update(const Resource& resource);
    void draw(Renderer& renderer);

private:
    int width;
    Resource current_resource;
};