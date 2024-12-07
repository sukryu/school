#include "ui/cursor.hpp"
#include "utils/utils.hpp"

namespace dune {
    namespace ui {

        Cursor::Cursor(types::Position initialPosition)
            : current_(initialPosition), previous_(initialPosition) {}

        void Cursor::move(types::Direction direction) {
            previous_ = current_;
            current_ = utils::move(current_, direction);
        }

        bool Cursor::isValidMove(types::Direction direction, const core::Map& map) const {
            types::Position newPos = utils::move(current_, direction);
            return newPos.row >= 0 && newPos.row < map.getHeight() - 2 &&
                newPos.column >= 0 && newPos.column < map.getWidth() - 2;
        }

        void Cursor::draw(Renderer& renderer, const core::Map& map) const {
            if (previous_ != current_) {
                restorePreviousPosition(renderer, map);
            }
            drawCursor(renderer);
        }

        void Cursor::restorePreviousPosition(Renderer& renderer, const core::Map& map) const {
            int mapX = previous_.column;
            int mapY = previous_.row;
            wchar_t ch = L' ';
            int color = constants::color::DEFAULT;

            const auto& terrain = map.getTerrainManager().getTerrain({ mapY, mapX });
            ch = terrain.getRepresentation();
            color = terrain.getColor();

            if (const auto* unit = map.getEntityAt<Unit>({ mapY, mapX })) {
                ch = unit->getRepresentation();
                color = unit->getColor();
            }
            else if (const auto* building = map.getEntityAt<Building>({ mapY, mapX })) {
                ch = building->getRepresentation();
                color = building->getColor();
            }

            int drawX = previous_.column + 1;
            int drawY = previous_.row + constants::RESOURCE_HEIGHT + 1;
            renderer.drawChar(drawX, drawY, ch, color);
        }

        void Cursor::drawCursor(Renderer& renderer) const {
            int drawX = current_.column + 1;
            int drawY = current_.row + constants::RESOURCE_HEIGHT + 1;
            renderer.drawChar(drawX, drawY, L' ', constants::color::CURSOR);
        }
    } // namespace ui
} // namespace dune
