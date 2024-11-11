#pragma once
#include "../utils/types.hpp"
#include "../managers/terrain_manager.hpp"
#include "../managers/building_manager.hpp"
#include "../managers/unit_manager.hpp"
#include <variant>

namespace dune {
    namespace core {

        class Selection {
        public:
            using Unit = managers::UnitManager::Unit;
            using Building = managers::BuildingManager::Building;
            using Terrain = managers::TerrainManager::Terrain;

            Selection() = default;

            void clear() {
                type = types::SelectionType::None;
                position = types::Position{ 0, 0 };
                selected_ptr = std::monostate{};
            }

            Unit* get_selected(Unit*) {
                if (auto* ptr = std::get_if<Unit*>(&selected_ptr)) {
                    return *ptr;
                }
                return nullptr;
            }

            const Unit* get_selected(Unit*) const {
                if (auto* ptr = std::get_if<Unit*>(&selected_ptr)) {
                    return *ptr;
                }
                return nullptr;
            }

            Building* get_selected(Building*) {
                if (auto* ptr = std::get_if<Building*>(&selected_ptr)) {
                    return *ptr;
                }
                return nullptr;
            }

            const Building* get_selected(Building*) const {
                if (auto* ptr = std::get_if<Building*>(&selected_ptr)) {
                    return *ptr;
                }
                return nullptr;
            }

            Terrain* get_selected(Terrain*) {
                if (auto* ptr = std::get_if<const Terrain*>(&selected_ptr)) {
                    return const_cast<Terrain*>(*ptr);
                }
                return nullptr;
            }

            const Terrain* get_selected(Terrain*) const {
                if (auto* ptr = std::get_if<const Terrain*>(&selected_ptr)) {
                    return *ptr;
                }
                return nullptr;
            }

            types::SelectionType get_type() const { return type; }
            types::Position get_position() const { return position; }

            types::SelectionType type = types::SelectionType::None;
            types::Position position = { 0, 0 };

        private:
            std::variant<std::monostate, const Terrain*, Building*, Unit*> selected_ptr;

            friend class Game;
        };

    } // namespace core
} // namespace dune