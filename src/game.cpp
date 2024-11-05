#include "game.hpp"
#include <thread>

Game::Game()
    : sys_clock(0),
    cursor({ 1, 1 }),
    resource{ 100, 1000, 10, 100 },  // 초기 자원 값 설정
    map(MAP_WIDTH, MAP_HEIGHT),
    display(MAP_WIDTH, MAP_HEIGHT, 30)
{
    init();
}

void Game::intro() {
    // 화면 클리어
    system("cls");

    // 인트로 메시지 출력
    IO::set_color(COLOR_DEFAULT);
    std::cout << "\n\n\n";
    std::cout << "        DUNE 1.5\n";
    std::cout << "\n        Loading...\n";

    // 잠시 대기
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 다시 화면 클리어
    system("cls");
}

void Game::outro() {
    // 화면 클리어
    system("cls");

    // 아웃트로 메시지 출력
    IO::set_color(COLOR_DEFAULT);
    std::cout << "\n\n\n";
    std::cout << "        게임을 종료합니다...\n";
    std::cout << "\n        Thank you for playing!\n";

    // 잠시 대기 후 종료
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    exit(0);
}

void Game::init() {
    init_resources();
    init_terrain();
    init_buildings();
    init_sandworms();
    init_harvesters();
    init_air_units();
    init_display();

    display.add_system_message("게임 초기화 완료");
}

void Game::init_resources() {
    resource = {
        .spice = 100,
        .spice_max = 1000,
        .population = 10,
        .population_max = 100
    };
}

void Game::init_terrain() {
    // 바위(Rock) 배치
    const std::vector<Position> rock_positions = {
        {5, 20}, {8, 30}, {12, 40}, {3, 50}, {15, 45}
    };

    for (const auto& pos : rock_positions) {
        map.set_terrain(pos, TerrainType::Rock);
    }

    // 스파이스 매장지 배치
    const std::vector<Position> spice_positions = {
        {15, 4}, {2, 55}
    };

    for (const auto& pos : spice_positions) {
        map.set_terrain(pos, TerrainType::Spice);
    }

    // 장판(Plate) 배치
    const std::vector<Position> plate_positions = {
        {MAP_HEIGHT - 3, 0}, {0, MAP_WIDTH - 3}
    };

    for (const auto& pos : plate_positions) {
        map.set_terrain(pos, TerrainType::Plate);
    }
}

void Game::init_buildings() {
    // Base(B)와 Plate(P) - 좌하단
    map.add_building(std::make_unique<BuildingManager::Building>(
        Camp::common, "Base", "본진", 0, Position{ MAP_HEIGHT - 4, 0 }, 2, 2, UnitType::harvester
    ));

    // Base(B)와 Plate(P) - 우상단
    map.add_building(std::make_unique<BuildingManager::Building>(
        Camp::common, "Base", "본진", 0, Position{ 0, MAP_WIDTH - 4 }, 2, 2, UnitType::harvester
    ));
}

void Game::init_sandworms() {
    // 아군 기지 근처 샌드웜
    map.add_unit(std::make_unique<UnitManager::Unit>(
        UnitType::sandworm,
        Position{ MAP_HEIGHT - 6, 5 }  // 좌하단 기지 근처
    ));

    // 적군 기지 근처 샌드웜
    map.add_unit(std::make_unique<UnitManager::Unit>(
        UnitType::sandworm,
        Position{ 5, MAP_WIDTH - 6 }  // 우상단 기지 근처
    ));
}

void Game::init_air_units() {
    for (int i = 0; i < 3; ++i) {
        Position pos = { rand() % MAP_HEIGHT, rand() % MAP_WIDTH };
        map.add_unit(std::make_unique<UnitManager::Unit>(
            UnitType::desert_eagle,
            pos
        ));
    }
}

void Game::init_harvesters() {
    const std::vector<Position> harvester_positions = {
        {MAP_HEIGHT - 5, 0},    // 좌하단 Base 위치
        {2, MAP_WIDTH - 3}     // 우상단 Base 위치
    };

    for (const auto& pos : harvester_positions) {
        map.add_unit(std::make_unique<UnitManager::Unit>(
            UnitType::harvester,
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

void Game::run() {
    intro();

    while (true) {
        Key key = IO::get_key();

        if (key == Key::Quit) {
            outro();
        }
        else if (key == Key::Space) {
            handle_selection();
        }
        else if (key == Key::Esc) {
            handle_escape();
        }
        else if (is_arrow_key(key)) {
            handle_movement(key);
        }

        // 모든 유닛 업데이트
        map.get_unit_manager().update_all_units(sys_clock, map);

        map.remove_destroyed_buildings();
        map.update(sys_clock);
        update_selection_display();
        display.update(resource, map, cursor);

        std::this_thread::sleep_for(std::chrono::milliseconds(TICK));
        sys_clock += std::chrono::milliseconds(TICK);
    }
}

void Game::handle_movement(Key key) {
    Direction dir;
    switch (key) {
    case Key::Up:    dir = Direction::Up;    break;
    case Key::Down:  dir = Direction::Down;  break;
    case Key::Left:  dir = Direction::Left;  break;
    case Key::Right: dir = Direction::Right; break;
    default: return;
    }

    int move_amount = IO::is_double_click() ? 10 : 1;

    for (int i = 0; i < move_amount; i++) {
        Position new_pos = ::move(cursor.get_current_position(), dir);
        if (new_pos.row >= 0 && new_pos.row < MAP_HEIGHT - 2 &&
            new_pos.column >= 0 && new_pos.column < MAP_WIDTH - 2) {
            cursor.move(dir);
        }
        else {
            break;
        }
    }
}

void Game::handle_selection() {
    Position pos = cursor.get_current_position();
    current_selection.position = pos;

    if (const Unit* unit = map.get_unit_at(pos)) {
        current_selection.type = SelectionType::Unit;
        current_selection.selected_ptr = const_cast<Unit*>(unit);  // const 캐스팅 추가
    }
    else if (const Building* building = map.get_building_at(pos)) {
        current_selection.type = SelectionType::Building;
        current_selection.selected_ptr = const_cast<Building*>(building);  // const 캐스팅 추가
    }
    else {
        current_selection.type = SelectionType::Terrain;
        current_selection.selected_ptr = const_cast<Terrain*>(&map.get_terrain_manager().get_terrain(pos));  // const 캐스팅 추가
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
    case SelectionType::Unit:
    {
        const Unit* unit = static_cast<const Unit*>(current_selection.selected_ptr);
        status_text = "Selected Unit: " + std::string(1, unit->get_representation());
        command_text = { "M: Move", "A: Attack", "S: Stop" };
    }
    break;
    case SelectionType::Building:
    {
        const Building* building = static_cast<const Building*>(current_selection.selected_ptr);
        status_text = "Selected Building: " + std::string(building->get_name());
        command_text = { "B: Build", "T: Train", "C: Cancel" };
    }
    break;
    case SelectionType::Terrain:
    {
        const Terrain* terrain = static_cast<const Terrain*>(current_selection.selected_ptr);
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