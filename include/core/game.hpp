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
        class Game {
        public:
            using Unit = managers::UnitManager::Unit;
            using Building = managers::BuildingManager::Building;
            using Terrain = managers::TerrainManager::Terrain;

            Game();
            void run();

        private:
            // 초기화 함수들
            void init();
            void init_resources();
            void init_buildings();
            void init_terrain();
            void init_sandworms();
            void init_air_units();
            void init_harvesters();
            void init_map();
            void init_display();

            // 게임 상태 관리
            void intro();
            void outro();
            void handle_movement(types::Key key);
            void handle_selection();
            /**
            * PDF 장판 설치
            */
            void handle_build_plate();
            types::Position find_empty_space_near_building(const Building* building);
            /**
            * PDF 4. 하베스터 생산
            */
            void handle_build_harvester(const Building* building);
            void handle_escape();
            void update_selection_display();
            void process_input();
            void update_game_state();
            void render();

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