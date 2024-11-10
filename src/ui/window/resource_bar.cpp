#include "../include/ui/window/resource_bar.hpp"

namespace dune {
    namespace ui {

        ResourceBar::ResourceBar(int width)
            : BaseWindow(0, 0, width, 1) {}

        void ResourceBar::update(const types::Resource& resource) {
            current_resource = resource;
        }

        std::string ResourceBar::format_resource_info() const {
            return "Spice: " + std::to_string(current_resource.spice) +
                "/" + std::to_string(current_resource.spice_max) +
                "  Pop: " + std::to_string(current_resource.population) +
                "/" + std::to_string(current_resource.population_max);
        }

        void ResourceBar::draw(Renderer& renderer) {
            std::string resource_info = format_resource_info();
            draw_text(renderer, 0, 0, resource_info, constants::color::RESOURCE);
        }
    } // namespace ui
} // namespace dune
