#pragma once
#include "map.hpp"
#include "../ui/cursor.hpp"
#include "../ui/display.hpp"
#include "../utils/types.hpp"
#include "../core/selection.hpp"
#include "io.hpp"
#include <chrono>
#include <memory>
#include <vector>

namespace dune {
	namespace core {
         /**
         * @brief 게임의 전체적인 흐름과 상태를 관리하는 클래스입니다.
         */
        class Game {
        public:
            using Unit = managers::UnitManager::Unit;
            using Building = managers::BuildingManager::Building;
            using Terrain = managers::TerrainManager::Terrain;

             /**
             * @brief Game 클래스의 생성자입니다.
             */
            Game();

            /**
             * @brief 게임 루프를 실행하여 게임을 진행합니다.
             */
            void run();

        private:
            // 초기화 함수들

            /**
             * @brief 게임 시작 시 필요한 초기화 작업을 수행합니다.
             */
            void init();

            /**
             * @brief 자원 정보를 초기화합니다.
             */
            void initResources();

            /**
             * @brief 초기 건물을 생성하고 배치합니다.
             */
            void initBuildings();

            /**
             * @brief 지형 정보를 초기화합니다.
             */
            void initTerrain();

            /**
             * @brief 샌드웜을 초기화합니다.
             */
            void initSandworms();

            /**
            * @brief 공중 유닛을 초기화합니다. (현재 미구현)
            */
            void initAirUnits();

            /**
            * @brief 하베스터 유닛을 초기화합니다.
            */
            void initHarvesters();

            /**
            * @brief 맵을 초기화합니다.
            */
            void initMap();

            /**
            * @brief 디스플레이를 초기화합니다.
            */
            void initDisplay();

            // 게임 상태 관리

            /**
             * @brief 게임 시작 시 인트로 화면을 표시합니다.
             */
            void intro();

            /**
             * @brief 게임 종료 시 아웃트로 화면을 표시합니다.
             */
            void outro();

            /**
             * @brief 사용자 입력에 따라 커서를 이동합니다.
             * @param key 입력된 키.
             */
            void handleMovement(types::Key key);

            /**
             * @brief 현재 커서 위치의 객체를 선택합니다.
             */
            void handleSelection();

            /**
             * @brief 장판을 설치합니다.
             */
            void handleBuildPlate();
            
            /**
             * @brief 숙소를 설치합니다.
             */
            void handleBuildDormitory();

            /**
             * @brief 창고를 설치합니다.
             */
            void handleBuildGarage();

            /**
             * @brief 병영을 설치합니다.
             */
            void handleBuildBarracks();

            /**
             * @brief 은신처을 설치합니다.
             */
            void handleBuildShelter();

            /**
             * @brief 투기장을 설치합니다.
             */
            void handleBuildArena();

            /**
             * @brief 공장을 설치합니다.
             */
            void handleBuildFactory();

            /**
             * @brief 건물 근처의 빈 공간을 찾습니다.
             * @param building 기준이 되는 건물 포인터.
             * @return types::Position 빈 공간의 위치.
             */
            types::Position findEmptySpaceNearBuilding(const Building* building);

            /**
             * @brief 하베스터를 생산합니다.
             * @param building 하베스터를 생산할 건물(본진).
             */
            void handleBuildHarvester(const Building* building);

            /**
             * @brief 보병을 생산합니다.
             * @param building 보병을 생상할 건물(병영).
             */
            void handleBuildSoldier(const Building* building);

            /**
             * @brief 프레멘을 생산합니다.
             * @param building 프레멘을 생상할 건물(은신처).
             */
            void handleBuildFremen(const Building* building);

            /**
             * @brief 투사을 생산합니다.
             * @param building 투사을 생상할 건물(투기장).
             */
            void handleBuildFighter(const Building* building);

            /**
             * @brief 중전차을 생산합니다.
             * @param building 중전차을 생상할 건물(공장).
             */
            void handleBuildH_Tank(const Building* building);

            /**
             * @brief 선택을 취소하거나 현재 작업을 종료합니다.
             */
            void handleEscape();

            /**
             * @brief 현재 선택된 객체에 따라 상태 표시를 업데이트합니다.
             */
            void updateSelectionDisplay();

            /**
             * @brief 사용자 입력을 처리합니다.
             */
            void processInput();

            /**
             * @brief 게임 상태를 업데이트합니다.
             */
            void updateGameState();

            /**
             * @brief 현재 게임 상태를 화면에 렌더링합니다.
             */
            void render();

            bool placeBuilding(std::unique_ptr<managers::BuildingManager::Building> building);

            void showUnitList();

            // 게임 상태
            std::chrono::milliseconds sys_clock;
            types::GameState game_state;
            Selection current_selection;

            // 게임 객체들
            ui::Display display;
            Map map;
            types::Resource resource;
            ui::Cursor cursor;
        };
	} // namespace core
} // namespace dune