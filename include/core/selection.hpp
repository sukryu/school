#pragma once
#include "../utils/types.hpp"
#include "../managers/terrain_manager.hpp"
#include "../managers/building_manager.hpp"
#include "../managers/unit_manager.hpp"
#include <variant>

namespace dune {
    namespace core {

        /**
         * @brief 현재 선택된 객체를 관리하는 클래스입니다.
         */
        class Selection {
        public:
            using Unit = managers::UnitManager::Unit;
            using Building = managers::BuildingManager::Building;
            using Terrain = managers::TerrainManager::Terrain;

            Selection() = default;

            /**
             * @brief 선택을 초기화합니다.
             */
            void clear() {
                type_ = types::SelectionType::None;
                position_ = { 0, 0 };
                selectedPtr_ = std::monostate{};
            }

            /**
             * @brief 선택된 객체를 반환하는 템플릿 함수입니다.
             * @tparam T 반환할 객체의 타입 (Unit, Building, Terrain).
             * @return T* 선택된 객체의 포인터.
             */
            template<typename T>
            const T* getSelected() const {
                if (auto* ptr = std::get_if<const T*>(&selectedPtr_)) {
                    return *ptr;
                }
                return nullptr;
            }

            /**
             * @brief 선택된 객체의 타입을 반환합니다.
             * @return types::SelectionType 선택 타입.
             */
            types::SelectionType getType() const { return type_; }

            /**
             * @brief 선택된 객체의 위치를 반환합니다.
             * @return types::Position 선택된 위치.
             */
            types::Position getPosition() const { return position_; }

        private:
            types::SelectionType type_ = types::SelectionType::None;
            types::Position position_ = { 0, 0 };
            std::variant<std::monostate, const Terrain*, const Building*, const Unit*> selectedPtr_;

            friend class Game;
        };

    } // namespace core
} // namespace dune
