#include "ui/window/base_window.hpp"
#include "utils/constants.hpp"

namespace dune {
    namespace ui {

        void BaseWindow::drawBorder(Renderer& renderer) const {
            // 상하 테두리
            for (int i = x_; i < x_ + width_; ++i) {
                renderer.drawChar(i, y_, L'#');
                renderer.drawChar(i, y_ + height_ - 1, L'#');
            }
            // 좌우 테두리
            for (int i = y_; i < y_ + height_; ++i) {
                renderer.drawChar(x_, i, L'#');
                renderer.drawChar(x_ + width_ - 1, i, L'#');
            }
        }

        void BaseWindow::drawText(Renderer& renderer, int relX, int relY,
            const std::wstring& text, int color) const {
            int drawX = x_ + relX;
            int drawY = y_ + relY;

            for (size_t i = 0; i < text.length() && i < static_cast<size_t>(width_ - relX); ++i) {
                renderer.drawChar(drawX + static_cast<int>(i), drawY, text[i], color);
            }
        }
    } // namespace ui
} // namespace dune
