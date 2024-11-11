#include "../include/ui/window/status_window.hpp"

namespace dune {
    namespace ui {

        StatusWindow::StatusWindow(int x, int y, int width, int height)
            : BaseWindow(x, y, width, height) {}

        void StatusWindow::update_status(const std::wstring& status) {
            status_text = status;
        }

        void StatusWindow::draw(Renderer& renderer) {
            draw_border(renderer);
            draw_text(renderer, 1, 1, status_text);
        }
    } // namespace ui
} // namespace dune