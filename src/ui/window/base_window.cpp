#include "ui/window/base_window.hpp"
#include "utils/constants.hpp"

namespace dune {
    namespace ui {

        void BaseWindow::draw_border(Renderer& renderer) const {
            // 상하 테두리
            for (int i = x; i < x + width; ++i) {
                renderer.draw_char(i, y, L'#');
                renderer.draw_char(i, y + height - 1, L'#');
            }
            // 좌우 테두리
            for (int i = y; i < y + height; ++i) {
                renderer.draw_char(x, i, L'#');
                renderer.draw_char(x + width - 1, i, L'#');
            }
        }

        void BaseWindow::draw_text(Renderer& renderer, int rel_x, int rel_y,
            const std::wstring& text, int color) const {
            int draw_x = x + rel_x;
            int draw_y = y + rel_y;

            for (size_t i = 0; i < text.length() && i < static_cast<size_t>(width - rel_x); ++i) {
                renderer.draw_char(draw_x + static_cast<int>(i), draw_y, text[i], color);
            }
        }
    } // namespace ui
} // namespace dune
