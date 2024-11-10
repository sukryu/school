#pragma once
#include "../utils/types.hpp"
#include "../core/map.hpp"
#include "../managers/unit_manager.hpp"
#include "../managers/building_manager.hpp"
#include "renderer.hpp"

namespace dune {
    namespace ui {

        class Cursor {
        public:
            using Unit = managers::UnitManager::Unit;
            using Building = managers::BuildingManager::Building;

            explicit Cursor(types::Position initial_position);
            void move(types::Direction dir);
            types::Position get_current_position() const { return current; }
            types::Position get_previous_position() const { return previous; }
            void draw(Renderer& renderer, const core::Map& map) const;
            bool is_valid_move(types::Direction dir, const core::Map& map) const;
            void set_position(types::Position pos) { previous = current; current = pos; }

        private:
            types::Position current;
            types::Position previous;
            void restore_previous_position(Renderer& renderer, const core::Map& map) const;
            void draw_cursor(Renderer& renderer) const;
        };

    } // namespace ui
} // namespace dune