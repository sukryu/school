#pragma once
#include "../core/entity.hpp"
#include "../utils/types.hpp"
#include "terrain_manager.hpp"
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

            /**
             * @brief 개별 건물을 나타내는 클래스입니다.
             */
            class Building : public core::Entity {
            public:
                /**
                 * @brief Building 클래스의 생성자입니다.
                 * @param type 건물의 진영 타입.
                 * @param name 건물의 이름.
                 * @param description 건물의 설명.
                 * @param buildCost 건설 비용.
                 * @param position 건물의 위치.
                 * @param width 건물의 가로 크기.
                 * @param height 건물의 세로 크기.
                 * @param producedUnit 생산 가능한 유닛 타입.
                 */
                Building(types::Camp type, const std::wstring& name, const std::wstring& description,
                         int buildCost, types::Position position, int width, int height,
                         types::UnitType producedUnit);

                // Entity 인터페이스 구현
                wchar_t getRepresentation() const override;
                int getColor() const override;
                void printInfo() const override;
                types::Position getPosition() const override { return position_; }
                void move(types::Direction direction) override;

                // Building 고유 메서드

                /**
                 * @brief 특정 위치가 건물의 영역 내에 있는지 확인합니다.
                 * @param position 확인할 위치.
                 * @return true 위치가 건물 내에 있으면 true.
                 * @return false 그렇지 않으면 false.
                 */
                bool contains(const types::Position& position) const;

                /**
                 * @brief 건물의 진영 타입을 반환합니다.
                 * @return types::Camp 진영 타입.
                 */
                types::Camp getType() const { return type_; }

                /**
                 * @brief 건물의 이름을 반환합니다.
                 * @return const std::wstring& 건물의 이름.
                 */
                const std::wstring& getName() const { return name_; }

                /**
                 * @brief 건물의 설명을 반환합니다.
                 * @return const std::wstring& 건물의 설명.
                 */
                const std::wstring& getDescription() const { return description_; }

                /**
                 * @brief 건물의 높이를 반환합니다.
                 * @return int 건물의 높이.
                 */
                int getHeight() const { return height_; }

                /**
                 * @brief 건물의 너비를 반환합니다.
                 * @return int 건물의 너비.
                 */
                int getWidth() const { return width_; }

                /**
                 * @brief 건물의 체력을 반환합니다.
                 * @return int 건물의 체력.
                 */
                int getHealth() const { return health_; }

                /**
                 * @brief 건물에 데미지를 입힙니다.
                 * @param damage 입힐 데미지 양.
                 */
                void takeDamage(int damage);

                /**
                 * @brief 건물이 파괴되었는지 확인합니다.
                 * @return true 체력이 0 이하이면 true.
                 * @return false 그렇지 않으면 false.
                 */
                bool isDestroyed() const { return health_ <= 0; }

                /**
                 * @brief 건물이 생산 가능한 유닛 타입을 반환합니다.
                 * @return types::UnitType 생산 가능한 유닛 타입.
                 */
                types::UnitType getProducedUnit() const { return producedUnit_; }

                /**
                 * @brief 해당 위치에 건물을 배치할 수 있는지 확인합니다.
                 * @param position 배치할 위치.
                 * @param terrainManager 지형 관리 객체.
                 * @return true 배치 가능하면 true.
                 * @return false 배치 불가능하면 false.
                 */
                bool isPlaceable(const types::Position& position, const TerrainManager& terrainManager) const;

            private:
                types::Camp type_;
                std::wstring name_;
                std::wstring description_;
                int buildingCost_;
                types::Position position_;
                int width_;
                int height_;
                types::UnitType producedUnit_;
                int health_;
            };

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
