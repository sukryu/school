#include "core/game.hpp"
#include "core/io.hpp"
#include "utils/utils.hpp"
#include <thread>
#include <array>
#include <map>
#include <iostream>
#include <regex>

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
                50,
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
                50,
                types::UnitType::Harvester
            ));
        }

        /**
        * PDF 3. 샌드웜
        */
        void Game::initSandworms() {
            auto sandworm1 = std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Sandworm,
                types::Position{ constants::MAP_HEIGHT - 6, 5 }
            );
            sandworm1->initializeAI(); // AI 초기화

            map.addUnit(std::move(sandworm1));

            auto sandworm2 = std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Sandworm,
                types::Position{ 5, constants::MAP_WIDTH - 6 }
            );
            sandworm2->initializeAI(); // AI 초기화

            map.addUnit(std::move(sandworm2));
        }

        void Game::addHarvester(const types::Position& pos, types::Camp camp) {
            auto harvester = std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Harvester, pos, camp
            );
            harvester->initializeAI();
            map.addUnit(std::move(harvester));

            std::wstring campName = (camp == types::Camp::ArtLadies) ? L"ArtLadies" : L"Harkonnen";
            display.addSystemMessage(campName + L" Harvester AI successfully initialized.");
        }

        /**
        * PDF 1. 준비
        */
        void Game::initHarvesters() {
            addHarvester({ constants::MAP_HEIGHT - 5, 0 }, types::Camp::ArtLadies);
            addHarvester({ 2, constants::MAP_WIDTH - 3 }, types::Camp::Harkonnen);
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
                if (auto* building = current_selection.getSelected<Building>()) {
                    if (building->getName() == L"Base" && key == types::Key::Build_Harvester) {
                        handleBuildHarvester(building);
                        return;
                    }
                    else if (key == types::Key::Build_Plate) {
                        handleBuildPlate();
                    }
                    else if (key == types::Key::Build_Dormitory) {
                        handleBuildDormitory();
                    }
                    else if (key == types::Key::Build_Garage) {
                        handleBuildGarage();
                    }
                    else if (key == types::Key::Build_Barracks) {
                        handleBuildBarracks();
                    }
                    else if (key == types::Key::Build_Shelter) {
                        handleBuildShelter();
                    }
                    else if (building->getName() == L"Barracks" && key == types::Key::Build_Soldier) {
                        handleBuildSoldier(building);
                    }
                    else if (building->getName() == L"Shelter" && key == types::Key::Build_Fremen) {
                        handleBuildFremen(building);
                    }
                    else if (building->getName() == L"Arena" && key == types::Key::Build_Fighter) {
                        handleBuildFighter(building);
                    }
                    else if (building->getName() == L"Factory" && key == types::Key::Build_HeavyTank) {
                        handleBuildH_Tank(building);
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
            else if (key == types::Key::ShowUnitList) {
                showUnitList();
            }
            else if (key == types::Key::Build_Arena) {
                handleBuildArena();
            }
            else if (key == types::Key::Build_Factory) {
                handleBuildFactory();
            }
            else if (utils::is_arrow_key(key)) {
                handleMovement(key);
            }
        }

        void Game::updateGameState() {
            map.update(sys_clock);
            map.removeDestroyedBuildings();

            // 하베스터의 스파이스 배달 처리
            const auto& messages = display.getMessageWindow().getMessages();
            for (const auto& message : messages) {
                // "Harvester returned with X spice." 메시지 확인
                static const std::wregex harvesterPattern(L"Harvester returned with (\\d+) spice\\.");
                std::wsmatch matches;
                if (std::regex_search(message.message, matches, harvesterPattern)) {
                    // 스파이스 양 추출
                    int spiceAmount = std::stoi(matches[1].str());

                    // 창고 용량 확인 및 자원 추가
                    int addedSpice = (spiceAmount < (resource.spice_max - resource.spice))
                        ? spiceAmount
                        : (resource.spice_max - resource.spice);
                    resource.spice += addedSpice;

                    // 창고가 가득 찼을 경우 메시지 표시
                    if (addedSpice < spiceAmount) {
                        display.addSystemMessage(
                            L"Storage full! Excess spice discarded: " +
                            std::to_wstring(spiceAmount - addedSpice)
                        );
                    }
                }
            }

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

            // 설치 가능 여부 확인
            auto& terrainManager = map.getTerrainManager();
            bool canPlace = true;

            // 2x2 크기의 장판을 설치할 수 있는지 확인
            for (int row = pos.row; row < pos.row + 2; ++row) {
                for (int col = pos.column; col < pos.column + 2; ++col) {
                    types::Position tilePos{ row, col };

                    // 맵 범위 및 지형 체크
                    if (!tilePos.is_valid() ||
                        terrainManager.getTerrain(tilePos).getType() != types::TerrainType::Desert) {
                        display.addSystemMessage(L"Cannot place Plate here.");
                        canPlace = false;
                        break;
                    }
                }
                if (!canPlace) break;
            }

            if (canPlace) {
                if (resource.spice < 1) {
                    display.addSystemMessage(L"Not enough spice to build Plate.");
                    return;
                }

                // Plate 설치
                for (int row = pos.row; row < pos.row + 2; ++row) {
                    for (int col = pos.column; col < pos.column + 2; ++col) {
                        terrainManager.setTerrain({ row, col }, types::TerrainType::Plate);
                    }
                }

                resource.spice -= 1;
                display.addSystemMessage(L"The Plate has been installed.");
            }
        }

        void Game::handleBuildDormitory() {
            types::Position pos = cursor.getCurrentPosition();

            auto Dormitory = std::make_unique<managers::BuildingManager::Building>(
                types::Camp::Common,
                L"Dormitory",
                L"숙소 (인구 최대치 증가 +10)",
                2,
                pos,
                2,
                2,
                10,
                types::UnitType::None
            );

            if (placeBuilding(std::move(Dormitory))) {
                if (resource.spice < 2) {
                    display.addSystemMessage(L"Not enough spice to build Dormitory");
                }
                else {
                    display.addSystemMessage(L"The Dormitory has been installed.");
                    resource.spice -= 2;
                    resource.population_max += 10;
                }
            }
            else {
                display.addSystemMessage(L"The Dormitory cannot be installed in this location.");
            }
        }

        void Game::handleBuildGarage() {
            types::Position pos = cursor.getCurrentPosition();

            auto garage = std::make_unique<managers::BuildingManager::Building>(
                types::Camp::Common,
                L"Garage",
                L"창고 (스파이스 최대치 증가 +10)",
                4,
                pos,
                2,
                2,
                10,
                types::UnitType::None
            );

            if (placeBuilding(std::move(garage))) {
                if (resource.spice < 4) {
                    display.addSystemMessage(L"Not enough spice to build garage");
                }
                else {
                    display.addSystemMessage(L"The garage has been installed.");
                    resource.spice -= 4;
                    resource.spice_max += 10;
                }
            }
            else {
                display.addSystemMessage(L"The garage cannot be installed in this location.");
            }
        }

        void Game::handleBuildBarracks() {
            types::Position pos = cursor.getCurrentPosition();

            auto barracks = std::make_unique<managers::BuildingManager::Building>(
                types::Camp::ArtLadies,
                L"Barracks",
                L"병영 (보병 생산)",
                4,
                pos,
                2,
                2,
                20,
                types::UnitType::Soldier
            );

            if (placeBuilding(std::move(barracks))) {
                if (resource.spice < 4) {
                    display.addSystemMessage(L"Not enough spice to build barracks");
                }
                else {
                    display.addSystemMessage(L"The barracks has been installed.");
                    resource.spice -= 4;
                }
            }
            else {
                display.addSystemMessage(L"The barracks cannot be installed in this location.");
            }
        }

        void Game::handleBuildShelter() {
            types::Position pos = cursor.getCurrentPosition();

            auto shelter = std::make_unique<managers::BuildingManager::Building>(
                types::Camp::ArtLadies,
                L"Shelter",
                L"은신처 (특수유닛 생산)",
                5,
                pos,
                2,
                2,
                30,
                types::UnitType::Fremen
            );

            if (placeBuilding(std::move(shelter))) {
                if (resource.spice < 5) {
                    display.addSystemMessage(L"Not enough spice to build shelter");
                }
                else {
                    display.addSystemMessage(L"The shelter has been installed.");
                    resource.spice -= 5;
                }
            }
            else {
                display.addSystemMessage(L"The shelter cannot be installed in this location.");
            }
        }

        void Game::handleBuildArena() {
            types::Position pos = cursor.getCurrentPosition();

            auto arena = std::make_unique<managers::BuildingManager::Building>(
                types::Camp::Harkonnen,
                L"Arena",
                L"투기장 (투사 생산)",
                4,
                pos,
                2,
                2,
                15,
                types::UnitType::Fighter
            );

            if (placeBuilding(std::move(arena))) {
                if (resource.spice < 3) {
                    display.addSystemMessage(L"Not enough spice to build arena");
                }
                else {
                    display.addSystemMessage(L"The arena has been installed.");
                    resource.spice -= 3;
                }
            }
            else {
                display.addSystemMessage(L"The arena cannot be installed in this location.");
            }
        }

        void Game::handleBuildFactory() {
            types::Position pos = cursor.getCurrentPosition();

            auto factory = std::make_unique<managers::BuildingManager::Building>(
                types::Camp::Harkonnen,
                L"Factory",
                L"공장 (중전차 생산)",
                5,
                pos,
                2,
                2,
                30,
                types::UnitType::HeavyTank
            );

            if (placeBuilding(std::move(factory))) {
                if (resource.spice < 5) {
                    display.addSystemMessage(L"Not enough spice to build factory");
                }
                else {
                    display.addSystemMessage(L"The factory has been installed.");
                    resource.spice -= 5;
                }
            }
            else {
                display.addSystemMessage(L"The factory cannot be installed in this location.");
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

            auto harvesterAI = std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Harvester,
                cursor_pos,
                building->getType() == types::Camp::ArtLadies ?
                types::Camp::ArtLadies : types::Camp::Harkonnen
            );

            // 하베스터 생성
            map.addUnit(std::move(harvesterAI));

            // 자원 소비
            resource.spice -= 5;
            resource.population += 5;
            display.addSystemMessage(L"A new harvester ready");
        }

        void Game::handleBuildSoldier(const Building* building) {
            // 자원 체크
            if (resource.spice < 1) {
                display.addSystemMessage(L"Not enough spice");
                return;
            }
            if (resource.population + 1 > resource.population_max) {
                display.addSystemMessage(L"Not enough population capacity");
                return;
            }

            // 커서 위치 가져오기
            types::Position cursor_pos = cursor.getCurrentPosition();
            types::Position base_pos = building->getPosition();

            // 본진과 커서 사이의 거리 계산
            int distance = utils::manhattanDistance(cursor_pos, base_pos);

            // 거리가 20 이상이면 설치 불가
            if (distance > 20) {
                display.addSystemMessage(L"Too far from the base");
                return;
            }

            // 설치 위치 유효성 검사
            if (!map.getTerrainManager().getTerrain(cursor_pos).isWalkable() ||
                map.getEntityAt<Unit>(cursor_pos) ||
                map.getEntityAt<Building>(cursor_pos)) {
                display.addSystemMessage(L"Cannot place Soldier here");
                return;
            }

            // 하베스터 생성
            map.addUnit(std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Soldier,
                1,      // build_cost
                1,      // population
                cursor_pos,
                15,     // health
                constants::SOLDIER_SPEED,
                5,      // attack_power
                1,      // sight_range
                types::Camp::ArtLadies
            ));

            // 자원 소비
            resource.spice -= 1;
            resource.population += 1;
            display.addSystemMessage(L"A new Soldier ready");
        }

        void Game::handleBuildFremen(const Building* building) {
            // 자원 체크
            if (resource.spice < 5) {
                display.addSystemMessage(L"Not enough spice");
                return;
            }
            if (resource.population + 2 > resource.population_max) {
                display.addSystemMessage(L"Not enough population capacity");
                return;
            }

            // 커서 위치 가져오기
            types::Position cursor_pos = cursor.getCurrentPosition();
            types::Position base_pos = building->getPosition();

            // 본진과 커서 사이의 거리 계산
            int distance = utils::manhattanDistance(cursor_pos, base_pos);

            // 거리가 20 이상이면 설치 불가
            if (distance > 20) {
                display.addSystemMessage(L"Too far from the base");
                return;
            }

            // 설치 위치 유효성 검사
            if (!map.getTerrainManager().getTerrain(cursor_pos).isWalkable() ||
                map.getEntityAt<Unit>(cursor_pos) ||
                map.getEntityAt<Building>(cursor_pos)) {
                display.addSystemMessage(L"Cannot place Fremen here");
                return;
            }

            // 하베스터 생성
            map.addUnit(std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Fremen,
                5,      // build_cost
                2,      // population
                cursor_pos,
                25,     // health
                constants::FREMEN_SPEED,
                15,      // attack_power
                8,      // sight_range
                types::Camp::ArtLadies
            ));

            // 자원 소비
            resource.spice -= 5;
            resource.population += 2;
            display.addSystemMessage(L"A new Fremen ready");
        }

        void Game::handleBuildFighter(const Building* building) {
            // 자원 체크
            if (resource.spice < 1) {
                display.addSystemMessage(L"Not enough spice");
                return;
            }
            if (resource.population + 1 > resource.population_max) {
                display.addSystemMessage(L"Not enough population capacity");
                return;
            }

            // 커서 위치 가져오기
            types::Position cursor_pos = cursor.getCurrentPosition();
            types::Position base_pos = building->getPosition();

            // 본진과 커서 사이의 거리 계산
            int distance = utils::manhattanDistance(cursor_pos, base_pos);

            // 거리가 20 이상이면 설치 불가
            if (distance > 20) {
                display.addSystemMessage(L"Too far from the base");
                return;
            }

            // 설치 위치 유효성 검사
            if (!map.getTerrainManager().getTerrain(cursor_pos).isWalkable() ||
                map.getEntityAt<Unit>(cursor_pos) ||
                map.getEntityAt<Building>(cursor_pos)) {
                display.addSystemMessage(L"Cannot place Soldier here");
                return;
            }

            // 하베스터 생성
            map.addUnit(std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::Fighter,
                1,      // build_cost
                1,      // population
                cursor_pos,
                10,     // health
                constants::FIGHTER_SPEED,
                6,      // attack_power
                1,      // sight_range
                types::Camp::Harkonnen
            ));

            // 자원 소비
            resource.spice -= 1;
            resource.population += 1;
            display.addSystemMessage(L"A new Fighter ready");
        }

        void Game::handleBuildH_Tank(const Building* building) {
            // 자원 체크
            if (resource.spice < 12) {
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
                display.addSystemMessage(L"Cannot place Heavy Tank here");
                return;
            }

            // 하베스터 생성
            map.addUnit(std::make_unique<managers::UnitManager::Unit>(
                types::UnitType::HeavyTank,
                12,      // build_cost
                5,      // population
                cursor_pos,
                60,     // health
                constants::HEAVY_TANK_SPEED,
                40,      // attack_power
                4,      // sight_range
                types::Camp::Harkonnen
            ));

            // 자원 소비
            resource.spice -= 12;
            resource.population += 5;
            display.addSystemMessage(L"A new Heavy Tank ready");
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
                current_selection.selectedPtr_ = unit; // const Unit*
            } else if (const Building* building = map.getEntityAt<Building>(pos)) {
                current_selection.type_ = types::SelectionType::Building;
                current_selection.selectedPtr_ = building; // const Building*
            } else {
                current_selection.type_ = types::SelectionType::Terrain;
                const Terrain& terrain = map.getTerrainManager().getTerrain(pos);
                current_selection.selectedPtr_ = &terrain; // const Terrain*
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
                if (auto* building = current_selection.getSelected<Building>()) {
                    status_text = L"Selected Building: " + building->getName();
                    if (building->getName() == L"Base") {
                        command_text = { 
                            L"H: Produce Harvester", 
                            L"Shift + P: Place Plate",
                            L"Shift + D: Place Dormitory",
                            L"Shift + G: Place Garage",
                            L"Shift + K: Place Barracks",
                            L"Shift + S: Place Shelter",
                            L"ESC: Cancel"
                        };
                    }
                    else if (building->getName() == L"Barracks") {
                        command_text = { L"S: Produce Soldier", L"ESC: Cancel" };
                    }
                    else  if (building->getName() == L"Shelter") {
                        command_text = { L"F: Produce Fremen", L"ESC: Cancel" };
                    }
                    else if (building->getName() == L"Arena") {
                        command_text = { L"R: Produce Fighter", L"ESC: Cancel" };
                    }
                    else if (building->getName() == L"Factory") {
                        command_text = { L"T: Produce heavy Tank", L"ESC: Cancel" };
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

        bool Game::placeBuilding(std::unique_ptr<managers::BuildingManager::Building> building) {
            if (building->getName() == L"Plate") {
                map.addBuilding(std::move(building));
                return true; 
            };
            auto pos = building->getPosition();
            const auto& terrainManager = map.getTerrainManager();

            // 설치 위치 확인 (건물이 설치 가능한 위치인지 확인)
            if (!building->isPlaceable(pos, terrainManager)) {
                return false;
            }

            // **장판 확인 로직 추가**
            for (int row = pos.row; row < pos.row + building->getHeight(); ++row) {
                for (int col = pos.column; col < pos.column + building->getWidth(); ++col) {
                    types::Position tilePos{ row, col };

                    // 장판이 없는 경우 설치 불가
                    if (terrainManager.getTerrain(tilePos).getType() != types::TerrainType::Plate) {
                        display.addSystemMessage(L"A Plate is required to build here.");
                        return false;
                    }
                }
            }
            // 건물을 맵에 추가
            map.addBuilding(std::move(building));
            return true;
        }

        void Game::showUnitList() {
            std::wcout << L"Executing Show Unit List" << std::endl; // 디버깅용
            std::map<wchar_t, int> unitCounts;
            std::wstring status_text = L"Unit List:\n";

            const auto& units = map.getUnitManager().getUnits();
            for (const auto& entry : units) {
                const auto& unit = entry.second;
                wchar_t representation = unit->getRepresentation();
                unitCounts[representation]++;
            }

            for (const auto& [representation, count] : unitCounts) {
                status_text += std::wstring(1, representation) + L": " + std::to_wstring(count) + L"\n";
            }

            display.updateStatus(status_text);
            std::wcout << status_text << std::endl; // 디버깅용
        }



    } // namespace core
} // namespace dune