#pragma once
#include "../utils/types.hpp"
#include "../utils/constants.hpp"
#include <vector>
#include <string>

namespace dune {
    namespace ui {

        class Renderer {
        public:
            Renderer(int width, int height);

            // 기본 그리기 함수들
            void clear();
            void draw_char(int x, int y, wchar_t ch, int color = constants::color::DEFAULT);
            void draw_string(int x, int y, const std::wstring& str, int color = constants::color::DEFAULT);
            void draw_border(int x, int y, int width, int height);
            void render();

            // 상태 확인
            wchar_t get_char_at(int x, int y) const;
            int get_color_at(int x, int y) const;
            bool is_valid_position(int x, int y) const;

            // 접근자
            int get_width() const { return width; }
            int get_height() const { return height; }

        private:
            int width;
            int height;
            std::vector<std::vector<wchar_t>> back_buffer;
            std::vector<std::vector<wchar_t>> front_buffer;
            std::vector<std::vector<int>> color_buffer;
            std::vector<std::vector<int>> front_color_buffer;

            void init_buffers();
            bool needs_update(int x, int y) const;
            void update_front_buffer(int x, int y);
        };
    } // namespace ui
} // namespace dune
