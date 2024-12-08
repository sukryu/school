#pragma once
#include "../managers/unit_manager.hpp"
#include "../managers/building_manager.hpp"
#include "../managers/terrain_manager.hpp"
#include "../ui/window/message_window.hpp"
#include "../utils/types.hpp"
#include <memory>
#include <chrono>
#include <string>

namespace dune {
    namespace core {

        /**
         * @brief 게임 맵과 관련된 모든 데이터를 관리하는 클래스입니다.
         */
        class Map {
        public:
            using Unit = managers::UnitManager::Unit;
            using Building = managers::BuildingManager::Building;
            using Terrain = managers::TerrainManager::Terrain;

            /**
             * @brief Map 클래스의 생성자입니다.
             * @param width 맵의 가로 크기.
             * @param height 맵의 세로 크기.
             * @param messageWindow 메시지 창 포인터.
             */
            Map(int width, int height, ui::MessageWindow* messageWindow);

            /**
             * @brief 특정 위치의 엔티티를 반환하는 템플릿 메서드입니다.
             * @tparam T 반환할 엔티티의 타입 (Unit, Building, Terrain).
             * @param position 확인할 위치.
             * @return const T* 해당 위치의 엔티티 포인터.
             */
            template<typename T>
            const T* getEntityAt(const types::Position& position) const {
                if constexpr (std::is_same_v<T, Unit>) {
                    return unitManager_.getUnitAt(position);
                } else if constexpr (std::is_same_v<T, Building>) {
                    return buildingManager_.getBuildingAt(position);
                } else if constexpr (std::is_same_v<T, Terrain>) {
                    return &terrainManager_.getTerrain(position);
                }
                return nullptr;
            }

            template<typename T>
            T* getEntityAt(const types::Position& position) {
                return const_cast<T*>(const_cast<const Map*>(this)->getEntityAt<T>(position));
            }

            /**
             * @brief 맵을 업데이트하고 필요한 동작을 수행합니다.
             * @param currentTime 현재 시간.
             */
            void update(std::chrono::milliseconds currentTime);

            /**
             * @brief 시스템 메시지를 추가합니다.
             * @param message 추가할 메시지.
             */
            void addSystemMessage(const std::wstring& message);

            // Manager 접근자
            const managers::TerrainManager& getTerrainManager() const { return terrainManager_; }
            managers::TerrainManager& getTerrainManager() { return terrainManager_; }
            const managers::UnitManager& getUnitManager() const { return unitManager_; }
            managers::UnitManager& getUnitManager() { return unitManager_; }
            const managers::BuildingManager& getBuildingManager() const { return buildingManager_; }

            // 객체 추가 함수

            /**
             * @brief 유닛을 맵에 추가합니다.
             * @param unit 추가할 유닛의 unique_ptr.
             */
            void addUnit(std::unique_ptr<Unit> unit);

            /**
             * @brief 건물을 맵에 추가합니다.
             * @param building 추가할 건물의 unique_ptr.
             */
            void addBuilding(std::unique_ptr<Building> building);

            /**
             * @brief 특정 위치의 지형 타입을 설정합니다.
             * @param position 설정할 위치.
             * @param type 지형 타입.
             */
            void setTerrain(const types::Position& position, types::TerrainType type);

            // 유닛 관련 함수

            /**
             * @brief 특정 위치에서 가장 가까운 유닛의 위치를 찾습니다.
             * @param fromPosition 시작 위치.
             * @param excludeType 제외할 유닛 타입.
             * @return types::Position 가장 가까운 유닛의 위치.
             */
            types::Position findNearestUnit(const types::Position& fromPosition, types::UnitType excludeType);

            /**
             * @brief 유닛을 제거합니다.
             * @param unit 제거할 유닛의 포인터.
             */
            void removeUnit(Unit* unit);

            // 건물 관련 함수

            /**
             * @brief 특정 위치의 건물에 데미지를 입힙니다.
             * @param position 건물의 위치.
             * @param damage 입힐 데미지 양.
             */
            void damageBuildingAt(const types::Position& position, int damage);

            /**
             * @brief 파괴된 건물을 제거합니다.
             */
            void removeDestroyedBuildings();

            // 유틸리티 함수

            /**
             * @brief 맵의 너비를 반환합니다.
             * @return int 맵의 너비.
             */
            int getWidth() const { return width_; }

            /**
             * @brief 맵의 높이를 반환합니다.
             * @return int 맵의 높이.
             */
            int getHeight() const { return height_; }



        private:
            void updateSandworm(Unit* unit, std::chrono::milliseconds currentTime);
            types::Position calculateSandwormMove(const Unit* sandworm, const types::Position& targetPosition);
            bool isValidSandwormTarget(const Unit* target) const;
            std::wstring getUnitTypeName(types::UnitType type) const;

            int width_;
            int height_;
            managers::TerrainManager terrainManager_;
            managers::UnitManager unitManager_;
            managers::BuildingManager buildingManager_;
            ui::MessageWindow* messageWindow_;  // Display의 MessageWindow를 참조
        };

    } // namespace core
} // namespace dune
