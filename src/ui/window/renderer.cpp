#include "../include/ui/renderer.hpp"
#include "../include/core/io.hpp"
#include "../include/utils/constants.hpp"
#include <algorithm>

namespace dune {
    namespace ui {

        Renderer::Renderer(int width, int height)
            : width(width)
            , height(height) {
            init_buffers();
        }

        void Renderer::init_buffers() {
            back_buffer.resize(height, std::vector<wchar_t>(width, L' '));
            front_buffer.resize(height, std::vector<wchar_t>(width, L' '));
            color_buffer.resize(height, std::vector<int>(width, constants::color::DEFAULT));
            front_color_buffer.resize(height, std::vector<int>(width, constants::color::DEFAULT));
        }

        void Renderer::clear() {
            for (int y = 0; y < height; ++y) {
                std::fill(back_buffer[y].begin(), back_buffer[y].end(), L' ');
                std::fill(color_buffer[y].begin(), color_buffer[y].end(), constants::color::DEFAULT);
            }
        }

        void Renderer::draw_char(int x, int y, wchar_t ch, int color) {
            if (is_valid_position(x, y)) {
                back_buffer[y][x] = ch;
                color_buffer[y][x] = color;
            }
        }

        void Renderer::draw_string(int x, int y, const std::wstring& str, int color) {
            for (size_t i = 0; i < str.length(); ++i) {
                if (is_valid_position(x + static_cast<int>(i), y)) {
                    draw_char(x + static_cast<int>(i), y, str[i], color);
                }
            }
        }

        void Renderer::draw_border(int x, int y, int width, int height) {
            for (int i = x; i < x + width; ++i) {
                draw_char(i, y, L'#');
                draw_char(i, y + height - 1, L'#');
            }
            for (int i = y; i < y + height; ++i) {
                draw_char(x, i, L'#');
                draw_char(x + width - 1, i, L'#');
            }
        }

        void Renderer::render() {
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    if (needs_update(x, y)) {
                        core::IO::printc({ y, x }, back_buffer[y][x], color_buffer[y][x]);
                        update_front_buffer(x, y);
                    }
                }
            }
        }

        bool Renderer::needs_update(int x, int y) const {
            return back_buffer[y][x] != front_buffer[y][x] ||
                color_buffer[y][x] != front_color_buffer[y][x];
        }

        void Renderer::update_front_buffer(int x, int y) {
            front_buffer[y][x] = back_buffer[y][x];
            front_color_buffer[y][x] = color_buffer[y][x];
        }

        wchar_t Renderer::get_char_at(int x, int y) const {
            return is_valid_position(x, y) ? back_buffer[y][x] : L' ';
        }

        int Renderer::get_color_at(int x, int y) const {
            return is_valid_position(x, y) ? color_buffer[y][x] : constants::color::DEFAULT;
        }

        bool Renderer::is_valid_position(int x, int y) const {
            return x >= 0 && x < width && y >= 0 && y < height;
        }

    } // namespace ui
} // namespace dune
