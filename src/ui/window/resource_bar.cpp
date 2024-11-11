#include "../include/ui/window/resource_bar.hpp"
#include <string>

namespace dune {
    namespace ui {

        ResourceBar::ResourceBar(int width)
            : BaseWindow(0, 0, width, 1) {}

        void ResourceBar::update(const types::Resource& resource) {
            current_resource = resource;
        }

        std::wstring ResourceBar::format_resource_info() const {
            return L"Spice: " + std::to_wstring(current_resource.spice) +
                L"/" + std::to_wstring(current_resource.spice_max) +
                L"  Pop: " + std::to_wstring(current_resource.population) +
                L"/" + std::to_wstring(current_resource.population_max);
        }

        void ResourceBar::draw(Renderer& renderer) {
            std::wstring resource_info = format_resource_info();
            draw_text(renderer, 0, 0, resource_info, constants::color::RESOURCE);
        }
    } // namespace ui
} // namespace dune
