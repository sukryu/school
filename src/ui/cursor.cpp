#include "ui/cursor.hpp"
#include "utils/utils.hpp"

/**
* PDF 2. 커서
*/
namespace dune {
    namespace ui {

        Cursor::Cursor(types::Position initial_position)
            : current(initial_position), previous(initial_position) {}

        void Cursor::move(types::Direction dir) {
            previous = current;
            current = utils::move(current, dir);
        }

        bool Cursor::is_valid_move(types::Direction dir, const core::Map& map) const {
            types::Position new_pos = utils::move(current, dir);
            return new_pos.row >= 0 && new_pos.row < map.get_height() - 2 &&
                new_pos.column >= 0 && new_pos.column < map.get_width() - 2;
        }

        void Cursor::draw(Renderer& renderer, const core::Map& map) const {
            if (previous != current) {
                restore_previous_position(renderer, map);
            }
            draw_cursor(renderer);
        }

        void Cursor::restore_previous_position(Renderer& renderer, const core::Map& map) const {
            int map_x = previous.column;
            int map_y = previous.row;
            wchar_t ch = L' ';
            int color = constants::color::DEFAULT;

            const auto& terrain = map.get_terrain_manager().get_terrain({ map_y, map_x });
            ch = terrain.get_representation();
            color = terrain.get_color();

            if (const auto* unit = map.get_entity_at<Unit>({ map_y, map_x })) {
                ch = unit->get_representation();
                color = unit->get_color();
            }
            else if (const auto* building = map.get_entity_at<Building>({ map_y, map_x })) {
                ch = building->get_representation();
                color = building->get_color();
            }

            int draw_x = previous.column + 1;
            int draw_y = previous.row + constants::RESOURCE_HEIGHT + 1;
            renderer.draw_char(draw_x, draw_y, ch, color);
        }

        void Cursor::draw_cursor(Renderer& renderer) const {
            int draw_x = current.column + 1;
            int draw_y = current.row + constants::RESOURCE_HEIGHT + 1;
            renderer.draw_char(draw_x, draw_y, L' ', constants::color::CURSOR);
        }
    } // namespace ui
} // namespace dune