#include "game.hpp"
#include <thread>

Game::Game()
    : sys_clock(0),
    cursor({ 1, 1 }),
    resource{ 100, 1000, 10, 100 },  // 초기 자원 값 설정
    map(MAP_WIDTH, MAP_HEIGHT),
    display(MAP_WIDTH, MAP_HEIGHT)
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
    init_buildings();
    init_terrain();
    init_sandworms();
    init_harvesters();
    init_resources();
    init_map();
    init_display();

    // 초기화 완료 메시지 표시
    display.add_system_message("게임 초기화 완료");
}

void Game::init_resources() {
    // 기본 자원 설정
    resource = {
        .spice = 100,        // 초기 스파이스
        .spice_max = 1000,   // 최대 스파이스 저장량
        .population = 10,    // 초기 인구
        .population_max = 100 // 최대 인구
    };
}

void Game::init_terrain() {
    // 1. 바위(Rock) 배치
    const std::vector<Position> rock_positions = {
        {5, 20},    // 중앙 좌측
        {8, 30},    // 중앙
        {12, 40},   // 중앙 우측
        {3, 50},    // 우측 상단
        {15, 45}    // 우측 하단
    };

    for (const auto& pos : rock_positions) {
        map.set_terrain(pos, TerrainType::Rock);
    }

    // 2. 스파이스 매장지 배치
    const std::vector<Position> spice_positions = {
        {15, 4},    // 좌측 하단
        {2, 55}     // 우측 상단
    };

    for (const auto& pos : spice_positions) {
        map.set_terrain(pos, TerrainType::Spice);
    }

    // 3. 장판(Plate) 배치
    const std::vector<Position> plate_positions = {
        {30, 30},    // 좌하단
        {30, 30}     // 우상단
    };

    for (const auto& pos : plate_positions) {
        map.set_terrain(pos, TerrainType::Plate);
    }

    // 나머지 영역은 기본적으로 사막(Desert)으로 설정되어 있음
}

void Game::init_buildings() {
    // Base(B)와 Plate(P) - 좌하단
    map.add_building(std::make_unique<Building>(
        Camp::common, "Base", "본진", 0, Position{ MAP_HEIGHT - 4, 0 }, 2, 2, UnitType::harvester
    ));

    // Base(B)와 Plate(P) - 우상단
    map.add_building(std::make_unique<Building>(
        Camp::common, "Base", "본진", 0, Position{ 0, MAP_WIDTH - 4 }, 2, 2, UnitType::harvester
    ));
}

void Game::init_sandworms() {
    const std::vector<Position> worm_positions = {
        {3, 15},    // 좌측 상단
        {12, 45}    // 우측 중앙
    };

    for (const auto& pos : worm_positions) {
        map.add_unit(std::make_unique<Unit>(
            UnitType::sandworm,
            0,      // build_cost
            0,      // population
            2500,   // speed
            10000,  // attack_power
            10000,  // health
            0,      // sight_range
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
        map.add_unit(std::make_unique<Unit>(
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

void Game::init_map() {
    // 기본 지형 설정
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            // 사막 지형 설정
            map.set_tile(0, { i, j }, ' ');
        }
    }
}

void Game::init_display() {
    // 디스플레이 초기 설정
    display.add_system_message("듄 1.5에 오신 것을 환영합니다");
    display.add_system_message("게임을 시작합니다");

    // 명령어 설명 추가
    display.add_command_message("방향키: 이동");
    display.add_command_message("B: 건설");
    display.add_command_message("Q: 종료");
}

void Game::run() {
    intro();

    while (true) {
        Key key = IO::get_key();

        if (is_arrow_key(key)) {
            cursor.move(key_to_direction(key));
        }
        else {
            switch (key) {
            case Key::Quit:
                outro();
                break;
            case Key::None:
            case Key::Undefined:
            default:
                break;
            }
        }

        map.update(sys_clock);
        display.update(resource, map, cursor);

        std::this_thread::sleep_for(std::chrono::milliseconds(TICK));
        sys_clock += std::chrono::milliseconds(TICK);
    }
}