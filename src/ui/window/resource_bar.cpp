#include "resource_bar.hpp"

namespace dune {
    namespace ui {

        ResourceBar::ResourceBar(int width)
            : BaseWindow(0, 0, width, 1) {}

        void ResourceBar::update(const types::Resource& resource) {
            currentResource_ = resource;
        }

        std::wstring ResourceBar::formatResourceInfo() const {
            return L"Spice: " + std::to_wstring(currentResource_.spice) +
                L"/" + std::to_wstring(currentResource_.spice_max) +
                L"  Pop: " + std::to_wstring(currentResource_.population) +
                L"/" + std::to_wstring(currentResource_.population_max);
        }

        void ResourceBar::draw(Renderer& renderer) {
            std::wstring resourceInfo = formatResourceInfo();
            drawText(renderer, 0, 0, resourceInfo, constants::color::RESOURCE);
        }
    } // namespace ui
} // namespace dune
