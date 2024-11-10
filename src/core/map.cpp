#include "../include/core/map.hpp"
#include "../include/ui/window/message_window.hpp"
#include "../include/utils/utils.hpp"

namespace dune {
    namespace core {

        Map::Map(int width, int height)
            : width(width)
            , height(height)
            , terrain_manager(width, height)
            , unit_manager()
            , building_manager()
            , message_window(nullptr) {}

        void Map::update(std::chrono::milliseconds current_time) {
            unit_manager.update_all_units(current_time, *this);
        }

        void Map::add_unit(std::unique_ptr<managers::UnitManager::Unit> unit) {
            unit_manager.add_unit(std::move(unit));
        }

        void Map::add_building(std::unique_ptr<managers::BuildingManager::Building> building) {
            building_manager.add_building(std::move(building));
        }

        void Map::set_terrain(const types::Position& pos, types::TerrainType type) {
            terrain_manager.set_terrain(pos, type);
        }

        types::Position Map::find_nearest_unit(const types::Position& from_pos, types::UnitType exclude_type) {
            types::Position nearest_pos = from_pos;
            int min_distance = std::numeric_limits<int>::max();

            for (const auto& unit : unit_manager.get_units()) {
                if (unit->get_type() != exclude_type) {
                    types::Position unit_pos = unit->get_position();
                    int distance = utils::manhattan_distance(unit_pos, from_pos);

                    if (distance < min_distance) {
                        min_distance = distance;
                        nearest_pos = unit_pos;
                    }
                }
            }
            return nearest_pos;
        }

        void Map::remove_unit(Unit* unit) {
            if (unit) {
                unit_manager.remove_unit(unit);
            }
        }

        void Map::damage_building_at(const types::Position& pos, int damage) {
            if (auto* building = building_manager.get_building_at(pos)) {
                building->take_damage(damage);
                if (building->is_destroyed()) {
                    add_system_message("건물이 파괴되었습니다!");
                }
            }
        }

        void Map::remove_destroyed_buildings() {
            building_manager.remove_destroyed_buildings();
        }

        void Map::add_system_message(const std::string& message) {
            if (message_window) {
                message_window->add_message(message);
            }
        }

    } // namespace core
} // namespace dune