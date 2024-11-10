#pragma once
#include "../utils/types.hpp"
#include <variant>

// 전방 선언
namespace dune {
    namespace managers {
        class TerrainManager;
        class BuildingManager;
        class UnitManager;
    }

    namespace core {

        class Selection {
        public:
            Selection() = default;

            void clear() {
                type = types::SelectionType::None;
                position = types::Position{ 0, 0 };
                selected_ptr = std::monostate{};
            }

            template<typename T>
            T* get_selected() {
                return std::get_if<T*>(&selected_ptr);
            }

            template<typename T>
            const T* get_selected() const {
                return std::get_if<T*>(&selected_ptr);
            }

            types::SelectionType get_type() const { return type; }
            types::Position get_position() const { return position; }

            types::SelectionType type = types::SelectionType::None;
            types::Position position = { 0, 0 };

        private:
            using TerrainPtr = const managers::TerrainManager::Terrain*;
            using BuildingPtr = managers::BuildingManager::Building*;
            using UnitPtr = managers::UnitManager::Unit*;

            std::variant<std::monostate, TerrainPtr, BuildingPtr, UnitPtr> selected_ptr;

            // friend 선언
            friend class Game;
            friend class managers::BuildingManager;
            friend class managers::UnitManager;
            friend class managers::TerrainManager;
        };

    } // namespace core
} // namespace dune