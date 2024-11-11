#pragma once
#include "renderer.hpp"
#include "../include/ui/window/message_window.hpp"
#include "../include/ui/window/map_renderer.hpp"
#include "../include/ui/window/command_window.hpp"
#include "../include/ui/window/resource_bar.hpp"
#include "../include/ui/window/status_window.hpp"
#include "cursor.hpp"
#include "../utils/types.hpp"
#include "../core/map.hpp"

namespace dune {
    namespace ui {

        class Display {
        public:
            Display(int map_width, int map_height, int status_width = constants::DEFAULT_STATUS_WIDTH);

            void update(const types::Resource& resource, const core::Map& map, const Cursor& cursor);
            void add_system_message(const std::wstring& message);
            void update_status(const std::wstring& status);
            void update_commands(const std::vector<std::wstring>& commands);

            // 접근자
            MessageWindow& get_message_window() { return message_window; }
            const MessageWindow& get_message_window() const { return message_window; }
            int get_total_width() const { return total_width; }
            int get_total_height() const { return total_height; }

        private:
            Renderer renderer;
            ResourceBar resource_bar;
            MessageWindow message_window;
            CommandWindow command_window;
            StatusWindow status_window;
            MapRenderer map_renderer;
            int total_width;
            int total_height;

            void render_all();
            void clear_screen();
        };
    } // namespace ui
} // namespace dune