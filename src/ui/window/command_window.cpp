#include "../include/ui/window/command_window.hpp"

namespace dune {
    namespace ui {

        CommandWindow::CommandWindow(int x, int y, int width, int height)
            : BaseWindow(x, y, width, height) {}

        void CommandWindow::update_commands(const std::vector<std::wstring>& new_commands) {
            commands = new_commands;
        }

        void CommandWindow::draw(Renderer& renderer) {
            draw_border(renderer);

            for (size_t i = 0; i < commands.size(); ++i) {
                draw_text(renderer, 3, 2 + static_cast<int>(i), commands[i]);
            }
        }
    } // namespace ui
} // namespace dune