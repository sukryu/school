#pragma once
#include "../renderer.hpp"

namespace dune {
	namespace ui {
        class BaseWindow {
        public:
            BaseWindow(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}

            virtual ~BaseWindow() = default;
            virtual void draw(Renderer& renderer) = 0;

            // 접근자
            int get_x() const { return x; }
            int get_y() const { return y; }
            int get_width() const { return width; }
            int get_height() const { return height; }

        protected:
            int x, y, width, height;

            void draw_border(Renderer& renderer) const;
            void draw_text(Renderer& renderer, int rel_x, int rel_y,
                const std::string& text, int color = constants::color::DEFAULT) const;
        };
	} // namespace ui
} // namespace dune