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
            Game();
            void run();

        private:
            using Unit = managers::UnitManager::Unit;
            using Building = managers::BuildingManager::Building;
            using Terrain = managers::TerrainManager::Terrain;

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
            Map map;
            types::Resource resource;
            ui::Cursor cursor;
            ui::Display display;
        };
	} // namespace core
} // namespace dune