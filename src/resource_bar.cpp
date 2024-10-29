#include "resource_bar.hpp"

ResourceBar::ResourceBar(int width) : width(width) {}

void ResourceBar::update(const Resource& resource) {
    current_resource = resource;
}

void ResourceBar::draw(Renderer& renderer) {
    std::string resource_info = "Spice: " + std::to_string(current_resource.spice) +
        "/" + std::to_string(current_resource.spice_max) +
        "  Pop: " + std::to_string(current_resource.population) +
        "/" + std::to_string(current_resource.population_max);

    for (size_t i = 0; i < resource_info.length() && i < static_cast<size_t>(width); ++i) {
        renderer.draw_char(static_cast<int>(i), 0, resource_info[i], COLOR_RESOURCE);
    }
}