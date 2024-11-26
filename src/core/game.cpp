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
            , map(constants::MAP_WIDTH, constants::MAP_HEIGHT, &display.getMessageWindow())
        {
            init();
        }

        /**
        * PDF 1. 준비
        */
        void Game::init() {
            initResources();
            initTerrain();
            initBuildings();
            initSandworms();
            initHarvesters();
            //init_air_units();
            initDisplay();

            display.addSystemMessage(L"Game initialization complete");
        }

        /**
        * PDF 1. 준비
        */
        void Game::initResources() {
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
        void Game::initTerrain() {
            // 바위(Rock) 배치
            const std::vector<types::Position> rock_positions = {
                {5, 20}, {8, 30}, {12, 40}, {3, 50}, {15, 45}
            };

            for (const auto& pos : rock_positions) {
                map.setTerrain(pos, types::TerrainType::Rock);
            }

            // 스파이스 매장지 배치
            const std::vector<types::Position> spice_positions = {
                {15, 4}, {2, 55}
            };

            for (const auto& pos : spice_positions) {
                map.setTerrain(pos, types::TerrainType::Spice);
            }

            // 장판(Plate) 배치
            const std::vector<types::Position> plate_positions = {
                {constants::MAP_HEIGHT - 3, 0}, {0, constants::MAP_WIDTH - 3}
            };

            for (const auto& pos : plate_positions) {
                map.setTerrain(pos, types::TerrainType::Plate);
            }
        }

        /**
        * PDF 1. 준비
        */
        void Game::initBuildings() {
            // Base(B)와 Plate(P) - 좌하단
            map.addBuilding(std::make_unique<managers::BuildingManager::Building>(
                types::Camp::ArtLadies,
                std::wstring(L"Base"),
                std::wstring(L"본진"),
                0,
                types::Position{ constants::MAP_HEIGHT - 4, 0 },
                2, 2,
                types::UnitType::Harvester
            ));

            // Base(B)와 Plate(P) - 우상단
            map.addBuilding(std::make_unique<managers::BuildingManager::Building>(
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
        void Game::initSandworms() {
            map.addUnit(std::make_unique<managers::Unit>(
                types::UnitType::Sandworm,
                types::Position{ constants::MAP_HEIGHT - 6, 5 }  // 좌하단 기지 근처
            ));

            map.addUnit(std::make_unique<managers::Unit>(
                types::UnitType::Sandworm,
                types::Position{ 5, constants::MAP_WIDTH - 6 }  // 우상단 기지 근처
            ));
        }

        /**
        * PDF 1. 준비
        */
        void Game::initHarvesters() {
            // 좌하단 하베스터 (아트레이디스 진영)
            map.addUnit(std::make_unique<managers::Unit>(
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
            map.addUnit(std::make_unique<managers::Unit>(
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
        void Game::initDisplay() {
            display.addSystemMessage(L"Welcome to Dune 1.5");
            display.addSystemMessage(L"Starting Game!");
            display.updateCommands({ L"Arrow keys: Move", L"Space: Select", L"Q: Exit" });
        }

        /**
        * PDF 1. 준비
        */
        void Game::intro() {
            IO::clearScreen();
            IO::setColor(constants::color::DEFAULT);

            std::wcout << L"\n\n\n";
            std::wcout << L"        DUNE 1.5\n";
            std::wcout << L"\n        Loading...\n";

            std::this_thread::sleep_for(std::chrono::seconds(2));
            IO::clearScreen();
        }

        /**
        * PDF 1. 준비
        */
        void Game::outro() {
            IO::clearScreen();
            IO::setColor(constants::color::DEFAULT);

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
                processInput();
                updateGameState();
                render();

                std::this_thread::sleep_for(std::chrono::milliseconds(constants::TICK));
                sys_clock += std::chrono::milliseconds(constants::TICK);
            }
        }

        void Game::processInput() {
            types::Key key = IO::getKey();

            if (current_selection.type_ == types::SelectionType::Building) {
                if (auto* building = current_selection.getSelected((Building*)nullptr)) {
                    if (building->getName() == L"Base" && key == types::Key::Build_Harvester) {
                        handleBuildHarvester(building);
                        return;
                    }
                }
            }

            if (key == types::Key::Quit) {
                game_state = types::GameState::GameOver;
                outro();
            }
            else if (key == types::Key::Space) {
                handleSelection();
            }
            else if (key == types::Key::Esc) {
                handleEscape();
            }
            else if (key == types::Key::Build_Plate) {
                handleBuildPlate();
            }
            else if (utils::is_arrow_key(key)) {
                handleMovement(key);
            }
        }

        void Game::updateGameState() {
            map.update(sys_clock);
            map.removeDestroyedBuildings();
            updateSelectionDisplay();
        }

        void Game::render() {
            display.update(resource, map, cursor);
        }

        void Game::handleMovement(types::Key key) {
            types::Direction dir = utils::key_to_direction(key);
            int move_amount = IO::isDoubleClick() ? 10 : 1;

            for (int i = 0; i < move_amount; i++) {
                types::Position new_pos = utils::move(cursor.getCurrentPosition(), dir);
                if (new_pos.row >= 0 && new_pos.row < constants::MAP_HEIGHT - 2 &&
                    new_pos.column >= 0 && new_pos.column < constants::MAP_WIDTH - 2) {
                    cursor.move(dir);
                }
                else {
                    break;
                }
            }
        }

        void Game::handleBuildPlate() {
            types::Position pos = cursor.getCurrentPosition();
            
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
            if (plate->isPlaceable(pos, map.getTerrainManager())) {
                map.addBuilding(std::move(plate));
                display.addSystemMessage(L"The board has been installed.");
            } else {
                display.addSystemMessage(L"The board cannot be installed in this location.");
            }
        }

        types::Position Game::findEmptySpaceNearBuilding(const Building* building) {
            const std::array<types::Position, 8> directions = { {
                {-1, 0}, {1, 0}, {0, -1}, {0, 1},  // 상하좌우
                {-1, -1}, {-1, 1}, {1, -1}, {1, 1}  // 대각선
            } };

            types::Position base_pos = building->getPosition();
            for (const auto& dir : directions) {
                types::Position check_pos{
                    base_pos.row + dir.row,
                    base_pos.column + dir.column
                };

                if (!map.getEntityAt<Unit>(check_pos) &&
                    !map.getEntityAt<Building>(check_pos) &&
                    map.getTerrainManager().getTerrain(check_pos).isWalkable()) {
                    return check_pos;
                }
            }
            return { -1, -1 };  // 유효하지 않은 위치
        }

        void Game::handleBuildHarvester(const Building* building) {
            // 자원 체크
            if (resource.spice < 5) {
                display.addSystemMessage(L"Not enough spice");
                return;
            }
            if (resource.population + 5 > resource.population_max) {
                display.addSystemMessage(L"Not enough population capacity");
                return;
            }

            // 커서 위치 가져오기
            types::Position cursor_pos = cursor.getCurrentPosition();
            types::Position base_pos = building->getPosition();

            // 본진과 커서 사이의 거리 계산
            int distance = utils::manhattanDistance(cursor_pos, base_pos);

            // 거리가 10 이상이면 설치 불가
            if (distance > 10) {
                display.addSystemMessage(L"Too far from the base");
                return;
            }

            // 설치 위치 유효성 검사
            if (!map.getTerrainManager().getTerrain(cursor_pos).isWalkable() ||
                map.getEntityAt<Unit>(cursor_pos) ||
                map.getEntityAt<Building>(cursor_pos)) {
                display.addSystemMessage(L"Cannot place harvester here");
                return;
            }

            // 하베스터 생성
            map.addUnit(std::make_unique<managers::Unit>(
                types::UnitType::Harvester,
                5,      // build_cost
                5,      // population
                constants::HARVESTER_SPEED,
                0,      // attack_power
                70,     // health
                0,      // sight_range
                cursor_pos,
                building->getType() == types::Camp::ArtLadies ?
                types::Camp::ArtLadies : types::Camp::Harkonnen
            ));

            // 자원 소비
            resource.spice -= 5;
            resource.population += 5;
            display.addSystemMessage(L"A new harvester ready");
        }

        void Game::handleSelection() {
            types::Position pos = cursor.getCurrentPosition();
            current_selection.position_ = pos;

            auto key = IO::getKey();
            if (key == types::Key::Build_Plate) {
                handleBuildPlate();
                return;
            }

            if (const Unit* unit = map.getEntityAt<Unit>(pos)) {
                current_selection.type_ = types::SelectionType::Unit;
                current_selection.selectedPtr_ = const_cast<core::Selection::Unit*>(unit);
            }
            else if (const Building* building = map.getEntityAt<Building>(pos)) {
                current_selection.type_ = types::SelectionType::Building;
                current_selection.selectedPtr_ = const_cast<Building*>(building);
            }
            else {
                current_selection.type_ = types::SelectionType::Terrain;
                current_selection.selectedPtr_ = &map.getTerrainManager().getTerrain(pos);
            }
        }

        void Game::handleEscape() {
            current_selection.clear();
            display.updateStatus(L"No Selection");
            display.updateCommands({ L"B: Build", L"T: Train", L"Q: Quit" });
        }

        void Game::updateSelectionDisplay() {
            std::wstring status_text;
            std::vector<std::wstring> command_text;

            switch (current_selection.type_) {
            case types::SelectionType::Unit:
                if (auto* unit = current_selection.getSelected<core::Selection::Unit>()) {
                    status_text = L"Selected Unit: " + std::wstring(1, unit->getRepresentation());
                    command_text = { L"M: Move", L"A: Attack", L"S: Stop" };
                }
                break;
            case types::SelectionType::Building:
                if (auto* building = current_selection.getSelected((Building*)nullptr)) {
                    status_text = L"Selected Building: " + building->getName();
                    if (building->getName() == L"Base") {
                        command_text = { L"H: Build Harvester", L"ESC: Cancel" };
                    }
                    else {
                        command_text = { L"No Actions Available" };
                    }
                }
                break;
            case types::SelectionType::Terrain:
                if (auto* terrain = current_selection.getSelected<core::Selection::Terrain>()) {
                    switch (terrain->getType()) {
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

                    if (terrain->canHarvestSpice()) {
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

            display.updateStatus(status_text);
            display.updateCommands(command_text);
        }
    } // namespace core
} // namespace dune