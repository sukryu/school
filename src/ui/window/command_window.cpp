#include "ui/window/command_window.hpp"

namespace dune {
    namespace ui {

        CommandWindow::CommandWindow(int x, int y, int width, int height)
            : BaseWindow(x, y, width, height) {}

        void CommandWindow::updateCommands(const std::vector<std::wstring>& newCommands) {
            commands_ = newCommands;
        }

        void CommandWindow::draw(Renderer& renderer) {
            drawBorder(renderer);

            for (size_t i = 0; i < commands_.size(); ++i) {
                drawText(renderer, 3, 2 + static_cast<int>(i), commands_[i]);
            }
        }
    } // namespace ui
} // namespace dune
