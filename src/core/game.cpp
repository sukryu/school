#include "core/game.hpp"
#include "core/io.hpp"
#include "utils/utils.hpp"
#include <thread>
#include <array>

namespace dune {
    namespace core {
        /**
        * PDF 1. 준비
        */
        Game::Game()
            : sys_clock(0)
            , game_state(types::GameState::Initial)
            , cursor({ 1, 1 })
            , resource{ 100, 1000, 10, 100 }
            , display(constants::MAP_WIDTH, constants::MAP_HEIGHT, constants::DEFAULT_STATUS_WIDTH)
            , map(constants::MAP_WIDTH, constants::MAP_HEIGHT, &display.get_message_window())
        {
            init();
        }

        /**
        * PDF 1. 준비
        */
        void Game::init() {
            init_resources();
            init_terrain();
            init_buildings();
            init_sandworms();
            init_harvesters();
            //init_air_units();
            init_display();

            display.add_system_message(L"Game initialization complete");
        }

        /**
        * PDF 1. 준비
        */
        void Game::init_resources() {
            resource = types::Resource{
                100,    // spice
                1000,   // spice_max
                10,     // population
                100     // population_max
            };
        }

        /**
        * PDF 1. 준비
        */
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

        /**
        * PDF 1. 준비
        */
        void Game::init_buildings() {
            // Base(B)와 Plate(P) - 좌하단
            map.add_building(std::make_unique<managers::BuildingManager::Building>(
                types::Camp::ArtLadies,
                std::wstring(L"Base"),
                std::wstring(L"본진"),
                0,
                types::Position{ constants::MAP_HEIGHT - 4, 0 },
                2, 2,
                types::UnitType::Harvester
            ));

            // Base(B)와 Plate(P) - 우상단
            map.add_building(std::make_unique<managers::BuildingManager::Building>(
                types::Camp::Harkonnen,
                std::wstring(L"Base"),
                std::wstring(L"적진"),
                0,
                types::Position{ 0, constants::MAP_WIDTH - 4 },
                2, 2,
                types::UnitType::Harvester
            ));
        }

        /**
        * PDF 3. 샌드웜
        */
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

        /**
        * PDF 1. 준비
        */
        void Game::init_harvesters() {
            // 좌하단 하베스터 (아트레이디스 진영)
            map.add_unit(std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Harvester,
                5,      // build_cost
                5,      // population
                constants::HARVESTER_SPEED,   // speed
                0,      // attack_power
                70,     // health
                0,      // sight_range
                types::Position{ constants::MAP_HEIGHT - 5, 0 },    // 좌하단 Base 위치
                types::Camp::ArtLadies  // 진영 추가
            ));

            // 우상단 하베스터 (하코넨 진영)
            map.add_unit(std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Harvester,
                5,      // build_cost
                5,      // population
                constants::HARVESTER_SPEED,   // speed
                0,      // attack_power
                70,     // health
                0,      // sight_range
                types::Position{ 2, constants::MAP_WIDTH - 3 },     // 우상단 Base 위치
                types::Camp::Harkonnen  // 진영 추가
            ));
        }

        /**
        * PDF 1. 준비
        */
        void Game::init_display() {
            display.add_system_message(L"Welcome to Dune 1.5");
            display.add_system_message(L"Starting Game!");
            display.update_commands({ L"Arrow keys: Move", L"Space: Select", L"Q: Exit" });
        }

        /**
        * PDF 1. 준비
        */
        void Game::intro() {
            IO::clear_screen();
            IO::set_color(constants::color::DEFAULT);

            std::wcout << L"\n\n\n";
            std::wcout << L"        DUNE 1.5\n";
            std::wcout << L"\n        Loading...\n";

            std::this_thread::sleep_for(std::chrono::seconds(2));
            IO::clear_screen();
        }

        /**
        * PDF 1. 준비
        */
        void Game::outro() {
            IO::clear_screen();
            IO::set_color(constants::color::DEFAULT);

            std::wcout << L"\n\n\n";
            std::wcout << L"        게임을 종료합니다...\n";
            std::wcout << L"\n        Thank you for playing!\n";

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

            if (current_selection.type == types::SelectionType::Building) {
                if (auto* building = current_selection.get_selected((Building*)nullptr)) {
                    if (building->get_name() == L"Base" && key == types::Key::Build_Harvester) {
                        handle_build_harvester(building);
                        return;
                    }
                }
            }

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
            else if (key == types::Key::Build_Plate) {
                handle_build_plate();
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

        void Game::handle_build_plate() {
            types::Position pos = cursor.get_current_position();
            
            // 2x2 크기의 장판 생성
            auto plate = std::make_unique<managers::BuildingManager::Building>(
                types::Camp::Common,
                L"Plate",
                L"건물 건설용 장판",
                0,  // 건설 비용
                pos,
                2,  // width
                2,  // height
                types::UnitType::None
            );

            // 설치 가능 여부 확인
            if (plate->is_placeable(pos, map.get_terrain_manager())) {
                map.add_building(std::move(plate));
                display.add_system_message(L"The board has been installed.");
            } else {
                display.add_system_message(L"The board cannot be installed in this location.");
            }
        }

        types::Position Game::find_empty_space_near_building(const Building* building) {
            const std::array<types::Position, 8> directions = { {
                {-1, 0}, {1, 0}, {0, -1}, {0, 1},  // 상하좌우
                {-1, -1}, {-1, 1}, {1, -1}, {1, 1}  // 대각선
            } };

            types::Position base_pos = building->get_position();
            for (const auto& dir : directions) {
                types::Position check_pos{
                    base_pos.row + dir.row,
                    base_pos.column + dir.column
                };

                if (!map.get_entity_at<Unit>(check_pos) &&
                    !map.get_entity_at<Building>(check_pos) &&
                    map.get_terrain_manager().get_terrain(check_pos).is_walkable()) {
                    return check_pos;
                }
            }
            return { -1, -1 };  // 유효하지 않은 위치
        }

        void Game::handle_build_harvester(const Building* building) {
            // 자원 체크
            if (resource.spice < 5) {
                display.add_system_message(L"Not enough spice");
                return;
            }
            if (resource.population + 5 > resource.population_max) {
                display.add_system_message(L"Not enough population capacity");
                return;
            }

            // 커서 위치 가져오기
            types::Position cursor_pos = cursor.get_current_position();
            types::Position base_pos = building->get_position();

            // 본진과 커서 사이의 거리 계산
            int distance = utils::manhattan_distance(cursor_pos, base_pos);

            // 거리가 10 이상이면 설치 불가
            if (distance > 10) {
                display.add_system_message(L"Too far from the base");
                return;
            }

            // 설치 위치 유효성 검사
            if (!map.get_terrain_manager().get_terrain(cursor_pos).is_walkable() ||
                map.get_entity_at<Unit>(cursor_pos) ||
                map.get_entity_at<Building>(cursor_pos)) {
                display.add_system_message(L"Cannot place harvester here");
                return;
            }

            // 하베스터 생성
            map.add_unit(std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Harvester,
                5,      // build_cost
                5,      // population
                constants::HARVESTER_SPEED,
                0,      // attack_power
                70,     // health
                0,      // sight_range
                cursor_pos,
                building->get_type() == types::Camp::ArtLadies ?
                types::Camp::ArtLadies : types::Camp::Harkonnen
            ));

            // 자원 소비
            resource.spice -= 5;
            resource.population += 5;
            display.add_system_message(L"A new harvester ready");
        }

        void Game::handle_selection() {
            types::Position pos = cursor.get_current_position();
            current_selection.position = pos;

            auto key = IO::get_key();
            if (key == types::Key::Build_Plate) {
                handle_build_plate();
                return;
            }

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
            display.update_status(L"No Selection");
            display.update_commands({ L"B: Build", L"T: Train", L"Q: Quit" });
        }

        void Game::update_selection_display() {
            std::wstring status_text;
            std::vector<std::wstring> command_text;

            switch (current_selection.type) {
            case types::SelectionType::Unit:
                if (auto* unit = current_selection.get_selected((Unit*)nullptr)) {
                    status_text = L"Selected Unit: " + std::wstring(1, unit->get_representation());
                    command_text = { L"M: Move", L"A: Attack", L"S: Stop" };
                }
                break;
            case types::SelectionType::Building:
                if (auto* building = current_selection.get_selected((Building*)nullptr)) {
                    status_text = L"Selected Building: " + building->get_name();
                    if (building->get_name() == L"Base") {
                        command_text = { L"H: Build Harvester", L"ESC: Cancel" };
                    }
                    else {
                        command_text = { L"No Actions Available" };
                    }
                }
                break;
            case types::SelectionType::Terrain:
                if (auto* terrain = current_selection.get_selected((Terrain*)nullptr)) {
                    switch (terrain->get_type()) {
                    case types::TerrainType::Desert:
                        status_text = L"Selected Terrain: Desert";
                        break;
                    case types::TerrainType::Plate:
                        status_text = L"Selected Terrain: Plate";
                        break;
                    case types::TerrainType::Rock:
                        status_text = L"Selected Terrain: Rock";
                        break;
                    case types::TerrainType::Spice:
                        status_text = L"Selected Terrain: Spice";
                        break;
                    default:
                        status_text = L"Selected Terrain: Unknown";
                    }

                    if (terrain->can_harvest_spice()) {
                        command_text = { L"H: Harvest Spice" };
                    }
                    else {
                        command_text = { L"No Actions Available" };
                    }
                }
                break;
            default:
                status_text = L"No Selection";
                command_text = { L"B: Build", L"T: Train", L"Q: Quit" };
                break;
            }

            display.update_status(status_text);
            display.update_commands(command_text);
        }
    } // namespace core
} // namespace dune