#include "ui/display.hpp"

namespace dune {
    namespace ui {

        Display::Display(int mapWidth, int mapHeight, int statusWidth)
            : renderer_(mapWidth + statusWidth,
                constants::RESOURCE_HEIGHT + mapHeight + constants::SYSTEM_MESSAGE_HEIGHT)
            , resourceBar_(mapWidth + statusWidth)
            , messageWindow_(0, constants::RESOURCE_HEIGHT + mapHeight, mapWidth, constants::SYSTEM_MESSAGE_HEIGHT)
            , commandWindow_(mapWidth, constants::RESOURCE_HEIGHT + mapHeight, statusWidth, constants::SYSTEM_MESSAGE_HEIGHT)
            , statusWindow_(mapWidth, constants::RESOURCE_HEIGHT, statusWidth, mapHeight)
            , mapRenderer_(0, constants::RESOURCE_HEIGHT, mapWidth, mapHeight)
            , totalWidth_(mapWidth + statusWidth)
            , totalHeight_(constants::RESOURCE_HEIGHT + mapHeight + constants::SYSTEM_MESSAGE_HEIGHT) {}

        void Display::update(const types::Resource& resource, const core::Map& map, const Cursor& cursor) {
            renderer_.clear();

            resourceBar_.update(resource);
            resourceBar_.draw(renderer_);

            mapRenderer_.draw(renderer_, map);

            statusWindow_.draw(renderer_);
            commandWindow_.draw(renderer_);
            messageWindow_.draw(renderer_);

            cursor.draw(renderer_, map);

            renderer_.render();
        }

        void Display::addSystemMessage(const std::wstring& message) {
            messageWindow_.addMessage(message);
        }

        void Display::updateStatus(const std::wstring& status) {
            statusWindow_.updateStatus(status);
        }

        void Display::updateCommands(const std::vector<std::wstring>& commands) {
            commandWindow_.updateCommands(commands);
        }

        void Display::clearScreen() {
            renderer_.clear();
        }
    } // namespace ui
} // namespace dune
