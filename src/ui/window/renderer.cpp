#include "ui/renderer.hpp"
#include "core/io.hpp"
#include "utils/constants.hpp"
#include <algorithm>

namespace dune {
    namespace ui {

        Renderer::Renderer(int width, int height)
            : width_(width)
            , height_(height) {
            initBuffers();
        }

        void Renderer::initBuffers() {
            backBuffer_.resize(height_, std::vector<wchar_t>(width_, L' '));
            frontBuffer_.resize(height_, std::vector<wchar_t>(width_, L' '));
            colorBuffer_.resize(height_, std::vector<int>(width_, constants::color::DEFAULT));
            frontColorBuffer_.resize(height_, std::vector<int>(width_, constants::color::DEFAULT));
        }

        void Renderer::clear() {
            for (int y = 0; y < height_; ++y) {
                std::fill(backBuffer_[y].begin(), backBuffer_[y].end(), L' ');
                std::fill(colorBuffer_[y].begin(), colorBuffer_[y].end(), constants::color::DEFAULT);
            }
        }

        void Renderer::drawChar(int x, int y, wchar_t ch, int color) {
            if (isValidPosition(x, y)) {
                backBuffer_[y][x] = ch;
                colorBuffer_[y][x] = color;
            }
        }

        void Renderer::drawString(int x, int y, const std::wstring& str, int color) {
            for (size_t i = 0; i < str.length(); ++i) {
                if (isValidPosition(x + static_cast<int>(i), y)) {
                    drawChar(x + static_cast<int>(i), y, str[i], color);
                }
            }
        }

        void Renderer::drawBorder(int x, int y, int width, int height) {
            for (int i = x; i < x + width; ++i) {
                drawChar(i, y, L'#');
                drawChar(i, y + height - 1, L'#');
            }
            for (int i = y; i < y + height; ++i) {
                drawChar(x, i, L'#');
                drawChar(x + width - 1, i, L'#');
            }
        }

        void Renderer::render() {
            for (int y = 0; y < height_; ++y) {
                for (int x = 0; x < width_; ++x) {
                    if (needsUpdate(x, y)) {
                        core::IO::printChar({ y, x }, backBuffer_[y][x], colorBuffer_[y][x]);
                        updateFrontBuffer(x, y);
                    }
                }
            }
        }

        bool Renderer::needsUpdate(int x, int y) const {
            return backBuffer_[y][x] != frontBuffer_[y][x] ||
                colorBuffer_[y][x] != frontColorBuffer_[y][x];
        }

        void Renderer::updateFrontBuffer(int x, int y) {
            frontBuffer_[y][x] = backBuffer_[y][x];
            frontColorBuffer_[y][x] = colorBuffer_[y][x];
        }

        wchar_t Renderer::getCharAt(int x, int y) const {
            return isValidPosition(x, y) ? backBuffer_[y][x] : L' ';
        }

        int Renderer::getColorAt(int x, int y) const {
            return isValidPosition(x, y) ? colorBuffer_[y][x] : constants::color::DEFAULT;
        }

        bool Renderer::isValidPosition(int x, int y) const {
            return x >= 0 && x < width_ && y >= 0 && y < height_;
        }

    } // namespace ui
} // namespace dune
