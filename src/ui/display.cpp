#include "../include/ui/display.hpp"

namespace dune {
    namespace ui {

        Display::Display(int map_width, int map_height, int status_width)
            : renderer(map_width + status_width,
                constants::RESOURCE_HEIGHT + map_height + constants::SYSTEM_MESSAGE_HEIGHT)
            , resource_bar(map_width + status_width)
            , message_window(0, constants::RESOURCE_HEIGHT + map_height, map_width, constants::SYSTEM_MESSAGE_HEIGHT)
            , command_window(map_width, constants::RESOURCE_HEIGHT + map_height, status_width, constants::SYSTEM_MESSAGE_HEIGHT)
            , status_window(map_width, constants::RESOURCE_HEIGHT, status_width, map_height)
            , map_renderer(0, constants::RESOURCE_HEIGHT, map_width, map_height)
            , total_width(map_width + status_width)
            , total_height(constants::RESOURCE_HEIGHT + map_height + constants::SYSTEM_MESSAGE_HEIGHT) {}

        void Display::update(const types::Resource& resource, const core::Map& map, const Cursor& cursor) {
            renderer.clear();

            resource_bar.update(resource);
            resource_bar.draw(renderer);

            map_renderer.draw(renderer, map);

            status_window.draw(renderer);
            command_window.draw(renderer);
            message_window.draw(renderer);

            cursor.draw(renderer, map);

            renderer.render();
        }

        void Display::add_system_message(const std::wstring& message) {
            message_window.add_message(message);
        }

        void Display::update_status(const std::wstring& status) {
            status_window.update_status(status);
        }

        void Display::update_commands(const std::vector<std::wstring>& commands) {
            command_window.update_commands(commands);
        }

        void Display::clear_screen() {
            renderer.clear();
        }
    } // namespace ui
} // namespace dune