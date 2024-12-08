#pragma once
#include "../core/entity.hpp"
#include "../utils/types.hpp"
#include "terrain_manager.hpp"
#include "entity/building.hpp"
#include <memory>
#include <vector>
#include <string>

namespace dune {
    namespace managers {

        /**
         * @brief 게임 내 건물을 관리하는 클래스입니다.
         */
        class BuildingManager {
        public:

            using Building = dune::entity::Building;

            /**
             * @brief 건물을 추가합니다.
             * @param building 추가할 건물의 unique_ptr.
             */
            void addBuilding(std::unique_ptr<Building> building);

            /**
             * @brief 특정 위치에 있는 건물을 반환합니다.
             * @param position 확인할 위치.
             * @return Building* 해당 위치에 있는 건물 포인터.
             */
            Building* getBuildingAt(const types::Position& position);

            /**
             * @brief 특정 위치에 있는 건물을 반환합니다. (상수 버전)
             * @param position 확인할 위치.
             * @return const Building* 해당 위치에 있는 건물 포인터.
             */
            const Building* getBuildingAt(const types::Position& position) const;

            /**
             * @brief 특정 건물을 제거합니다.
             * @param building 제거할 건물의 포인터.
             */
            void removeBuilding(Building* building);

            /**
             * @brief 현재 존재하는 모든 건물의 리스트를 반환합니다.
             * @return const std::vector<std::unique_ptr<Building>>& 건물 리스트.
             */
            const std::vector<std::unique_ptr<Building>>& getBuildings() const;

            /**
             * @brief 파괴된 건물을 제거합니다.
             */
            void removeDestroyedBuildings();

        private:
            std::vector<std::unique_ptr<Building>> buildings_;
        };

    } // namespace managers
} // namespace dune
