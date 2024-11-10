#pragma once
#include "../managers/unit_manager.hpp"
#include "../managers/building_manager.hpp"
#include "../managers/terrain_manager.hpp"
#include "../ui/window/message_window.hpp"
#include "../utils/types.hpp"
#include <memory>
#include <chrono>

namespace dune {
	namespace core {
        class Map {
        public:
            using Unit = managers::UnitManager::Unit;
            using Building = managers::BuildingManager::Building;
            using Terrain = managers::TerrainManager::Terrain;

            Map(int width, int height);

            // 템플릿 메서드
            template<typename T>
            const T* get_entity_at(const types::Position& pos) const {
                if constexpr (std::is_same_v<T, Unit>) {
                    return unit_manager.get_unit_at(pos);
                }
                else if constexpr (std::is_same_v<T, Building>) {
                    return building_manager.get_building_at(pos);
                }
                else if constexpr (std::is_same_v<T, Terrain>) {
                    return &terrain_manager.get_terrain(pos);
                }
                return nullptr;
            }

            template<typename T>
            T* get_entity_at(types::Position pos) {
                return const_cast<T*>(const_cast<const Map*>(this)->get_entity_at<T>(pos));
            }

            // Manager 접근자
            const managers::TerrainManager& get_terrain_manager() const { return terrain_manager; }
            const managers::UnitManager& get_unit_manager() const { return unit_manager; }
            managers::UnitManager& get_unit_manager() { return unit_manager; }
            const managers::BuildingManager& get_building_manager() const { return building_manager; }

            // 객체 추가 함수
            void add_unit(std::unique_ptr<Unit> unit);
            void add_building(std::unique_ptr<Building> building);
            void set_terrain(const types::Position& pos, types::TerrainType type);

            // 유닛 관련 함수
            types::Position find_nearest_unit(const types::Position& from_pos, types::UnitType exclude_type);
            void remove_unit(Unit* unit);

            // 건물 관련 함수
            void damage_building_at(const types::Position& pos, int damage);
            void remove_destroyed_buildings();

            // 업데이트 및 메시지
            void update(std::chrono::milliseconds current_time);
            void set_message_window(ui::MessageWindow* msg_window) { message_window = msg_window; }
            void add_system_message(const std::string& message);

            // 유틸리티 함수
            int get_width() const { return width; }
            int get_height() const { return height; }

        private:
            int width;
            int height;
            managers::TerrainManager terrain_manager;
            managers::UnitManager unit_manager;
            managers::BuildingManager building_manager;
            ui::MessageWindow* message_window;
        };
	} // namespace core
} // namespace dune