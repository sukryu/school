#include "../include/ui/window/message_window.hpp"

namespace dune {
    namespace ui {

        MessageWindow::MessageWindow(int x, int y, int width, int height)
            : BaseWindow(x, y, width, height) {}

        void MessageWindow::add_message(const std::string& message) {
            messages.push_back(message);
            if (messages.size() > MAX_MESSAGES) {
                messages.pop_front();
            }
        }

        void MessageWindow::draw(Renderer& renderer) {
            draw_border(renderer);

            int message_y = 2;
            for (const auto& message : messages) {
                draw_text(renderer, 3, message_y, message);
                ++message_y;
            }
        }
    } // namespace ui
} // namespace dune