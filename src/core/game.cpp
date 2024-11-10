#include "core/game.hpp"
#include "core/io.hpp"
#include "utils/utils.hpp"
#include <thread>

namespace dune {
    namespace core {

        Game::Game()
            : sys_clock(0)
            , game_state(types::GameState::Initial)
            , cursor({ 1, 1 })
            , resource{ 100, 1000, 10, 100 }
            , map(constants::MAP_WIDTH, constants::MAP_HEIGHT)
            , display(constants::MAP_WIDTH, constants::MAP_HEIGHT, constants::DEFAULT_STATUS_WIDTH)
        {
            init();
        }

        void Game::init() {
            init_resources();
            init_terrain();
            init_buildings();
            init_sandworms();
            init_harvesters();
            //init_air_units();
            init_display();

            display.add_system_message("게임 초기화 완료");
        }

        void Game::init_resources() {
            resource = types::Resource{
                100,    // spice
                1000,   // spice_max
                10,     // population
                100     // population_max
            };
        }

        void Game::init_terrain() {
            // 바위(Rock) 배치
            const std::vector<types::Position> rock_positions = {
                {5, 20}, {8, 30}, {12, 40}, {3, 50}, {15, 45}
            };

            for (const auto& pos : rock_positions) {
                map.set_terrain(pos, types::TerrainType::Rock);
            }

            // 스파이스 매장지 배치
            const std::vector<types::Position> spice_positions = {
                {15, 4}, {2, 55}
            };

            for (const auto& pos : spice_positions) {
                map.set_terrain(pos, types::TerrainType::Spice);
            }

            // 장판(Plate) 배치
            const std::vector<types::Position> plate_positions = {
                {constants::MAP_HEIGHT - 3, 0}, {0, constants::MAP_WIDTH - 3}
            };

            for (const auto& pos : plate_positions) {
                map.set_terrain(pos, types::TerrainType::Plate);
            }
        }

        void Game::init_buildings() {
            // Base(B)와 Plate(P) - 좌하단
            map.add_building(std::make_unique<managers::BuildingManager::Building>(
                types::Camp::Common,
                std::string("Base"),
                std::string("본진"),
                0,
                types::Position{ constants::MAP_HEIGHT - 4, 0 },
                2, 2,
                types::UnitType::Harvester
            ));

            // Base(B)와 Plate(P) - 우상단
            map.add_building(std::make_unique<managers::BuildingManager::Building>(
                types::Camp::Common,
                std::string("Base"),
                std::string("본진"),
                0,
                types::Position{ 0, constants::MAP_WIDTH - 4 },
                2, 2,
                types::UnitType::Harvester
            ));
        }

        void Game::init_sandworms() {
            map.add_unit(std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Sandworm,
                types::Position{ constants::MAP_HEIGHT - 6, 5 }  // 좌하단 기지 근처
            ));

            map.add_unit(std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Sandworm,
                types::Position{ 5, constants::MAP_WIDTH - 6 }  // 우상단 기지 근처
            ));
        }

        void Game::init_harvesters() {
            const std::vector<types::Position> harvester_positions = {
                {constants::MAP_HEIGHT - 5, 0},    // 좌하단 Base 위치
                {2, constants::MAP_WIDTH - 3}      // 우상단 Base 위치
            };

            for (const auto& pos : harvester_positions) {
                map.add_unit(std::make_unique<managers::UnitManager::Unit>(
                    types::UnitType::Harvester,
                    5,      // build_cost
                    5,      // population
                    2000,   // speed
                    0,      // attack_power
                    70,     // health
                    0,      // sight_range
                    pos
                ));
            }
        }

        void Game::init_display() {
            display.add_system_message("듄 1.5에 오신 것을 환영합니다");
            display.add_system_message("게임을 시작합니다");
            display.update_commands({ "방향키: 이동", "Space: 선택", "Q: 종료" });
        }

        void Game::intro() {
            IO::clear_screen();
            IO::set_color(constants::color::DEFAULT);

            std::cout << "\n\n\n";
            std::cout << "        DUNE 1.5\n";
            std::cout << "\n        Loading...\n";

            std::this_thread::sleep_for(std::chrono::seconds(2));
            IO::clear_screen();
        }

        void Game::outro() {
            IO::clear_screen();
            IO::set_color(constants::color::DEFAULT);

            std::cout << "\n\n\n";
            std::cout << "        게임을 종료합니다...\n";
            std::cout << "\n        Thank you for playing!\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::exit(0);
        }

        void Game::run() {
            intro();
            game_state = types::GameState::Running;

            while (game_state == types::GameState::Running) {
                process_input();
                update_game_state();
                render();

                std::this_thread::sleep_for(std::chrono::milliseconds(constants::TICK));
                sys_clock += std::chrono::milliseconds(constants::TICK);
            }
        }

        void Game::process_input() {
            types::Key key = IO::get_key();

            if (key == types::Key::Quit) {
                game_state = types::GameState::GameOver;
                outro();
            }
            else if (key == types::Key::Space) {
                handle_selection();
            }
            else if (key == types::Key::Esc) {
                handle_escape();
            }
            else if (utils::is_arrow_key(key)) {
                handle_movement(key);
            }
        }

        void Game::update_game_state() {
            map.update(sys_clock);
            map.remove_destroyed_buildings();
            update_selection_display();
        }

        void Game::render() {
            display.update(resource, map, cursor);
        }

        void Game::handle_movement(types::Key key) {
            types::Direction dir = utils::key_to_direction(key);
            int move_amount = IO::is_double_click() ? 10 : 1;

            for (int i = 0; i < move_amount; i++) {
                types::Position new_pos = utils::move(cursor.get_current_position(), dir);
                if (new_pos.row >= 0 && new_pos.row < constants::MAP_HEIGHT - 2 &&
                    new_pos.column >= 0 && new_pos.column < constants::MAP_WIDTH - 2) {
                    cursor.move(dir);
                }
                else {
                    break;
                }
            }
        }

        void Game::handle_selection() {
            types::Position pos = cursor.get_current_position();
            current_selection.position = pos;

            if (const Unit* unit = map.get_entity_at<Unit>(pos)) {
                current_selection.type = types::SelectionType::Unit;
                current_selection.selected_ptr = const_cast<Unit*>(unit);
            }
            else if (const Building* building = map.get_entity_at<Building>(pos)) {
                current_selection.type = types::SelectionType::Building;
                current_selection.selected_ptr = const_cast<Building*>(building);
            }
            else {
                current_selection.type = types::SelectionType::Terrain;
                current_selection.selected_ptr = &map.get_terrain_manager().get_terrain(pos);
            }
        }

        void Game::handle_escape() {
            current_selection.clear();
            display.update_status("No Selection");
            display.update_commands({ "B: Build", "T: Train", "Q: Quit" });
        }

        void Game::update_selection_display() {
            std::string status_text;
            std::vector<std::string> command_text;

            switch (current_selection.type) {
            case types::SelectionType::Unit:
                if (auto* unit = current_selection.get_selected<Unit>()) {
                    status_text = "Selected Unit: " + std::string(1, unit->get_representation());
                    command_text = { "M: Move", "A: Attack", "S: Stop" };
                }
                break;
            case types::SelectionType::Building:
                if (auto* building = current_selection.get_selected<Building>()) {
                    status_text = "Selected Building: " + building->get_name();
                    command_text = { "B: Build", "T: Train", "C: Cancel" };
                }
                break;
            case types::SelectionType::Terrain:
                if (auto* terrain = current_selection.get_selected<Terrain>()) {
                    status_text = "Selected Terrain: " + std::string(1, terrain->get_representation());
                    if (terrain->can_harvest_spice()) {
                        command_text = { "H: Harvest Spice" };
                    }
                    else {
                        command_text = { "No Actions Available" };
                    }
                }
                break;
            default:
                status_text = "No Selection";
                command_text = { "B: Build", "T: Train", "Q: Quit" };
                break;
            }

            display.update_status(status_text);
            display.update_commands(command_text);
        }

    } // namespace core
} // namespace dune