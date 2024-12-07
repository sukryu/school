#include "ui/window/status_window.hpp"

namespace dune {
    namespace ui {

        StatusWindow::StatusWindow(int x, int y, int width, int height)
            : BaseWindow(x, y, width, height) {}

        void StatusWindow::updateStatus(const std::wstring& status) {
            statusText_ = status;
        }

        void StatusWindow::draw(Renderer& renderer) {
            drawBorder(renderer);
            drawText(renderer, 1, 1, statusText_);
        }
    } // namespace ui
} // namespace dune
