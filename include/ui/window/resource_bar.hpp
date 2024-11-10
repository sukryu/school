#pragma once
#include "base_window.hpp"
#include "../../utils/types.hpp"

namespace dune {
    namespace ui {

        class ResourceBar : public BaseWindow {
        public:
            explicit ResourceBar(int width);
            void update(const types::Resource& resource);
            void draw(Renderer& renderer) override;

        private:
            types::Resource current_resource;
            std::string format_resource_info() const;
        };
    } // namespace ui
} // namespace dune