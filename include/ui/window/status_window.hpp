#pragma once
#include "base_window.hpp"
#include <string>

namespace dune {
    namespace ui {

        class StatusWindow : public BaseWindow {
        public:
            StatusWindow(int x, int y, int width, int height);
            void update_status(const std::wstring& status);
            void draw(Renderer& renderer) override;

            const std::wstring& get_current_status() const { return status_text; }

        private:
            std::wstring status_text;
        };
    } // namespace ui
} // namespace dune