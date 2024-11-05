#include "unit_manager.hpp"
#include "map.hpp"
#include "message_window.hpp"
#include <iostream>

using Building = BuildingManager::Building;

UnitManager::Unit::Unit(UnitType type, int build_cost, int population, int speed,
    int attack_power, int health, int sight_range, Position pos)
    : type(type), build_cost(build_cost), population(population), speed(speed),
    attack_power(attack_power), health(health), sight_range(sight_range), pos(pos) {
}

UnitManager::Unit::Unit(UnitType type, Position pos)
    : type(type), pos(pos) {
    switch (type) {
    case UnitType::sandworm:
        build_cost = 0;
        population = 0;
        speed = 500;      // 천천히 움직임
        attack_power = 9999;
        health = 100;
        sight_range = 999;
        length = 1;
        break;
    case UnitType::desert_eagle:
        build_cost = 0;
        population = 0;
        speed = 200;      // 빠르게 움직임
        attack_power = 0;
        health = 10;
        sight_range = 5;
        break;
    case UnitType::sandstorm:
        build_cost = 0;
        population = 0;
        speed = 100;      // 매우 빠르게 움직임
        attack_power = 0;
        health = 1;
        sight_range = 2;
        creation_time = std::chrono::milliseconds(0);
        break;
    default:
        build_cost = 5;
        population = 2;
        speed = 1000;
        attack_power = 10;
        health = 10;
        sight_range = 3;
        break;
    }
}

void UnitManager::Unit::check_and_damage_area(Map& map, const Position& pos) {
    for (int i = 0; i < SANDSTORM_SIZE; ++i) {
        for (int j = 0; j < SANDSTORM_SIZE; ++j) {
            Position check_pos = { pos.row + i, pos.column + j };

            // 유닛 제거
            Unit* unit = map.get_unit_at(check_pos);
            if (unit && unit != this) {
                map.add_system_message("모래 폭풍이 유닛을 파괴했습니다!");
                map.remove_unit(unit);
            }

            // 건물 데미지
            Building* building = map.get_building_at(check_pos);
            if (building) {
                building->take_damage(50);
                map.add_system_message("모래 폭풍이 건물에 데미지를 입혔습니다!");
            }
        }
    }
}

char UnitManager::Unit::get_representation() const {
    switch (type) {
    case UnitType::harvester: return 'H';
    case UnitType::fremen: return 'M';
    case UnitType::soldier: return 'I';
    case UnitType::fighter: return 'J';
    case UnitType::heavy_tank: return 'T';
    case UnitType::sandworm: return 'W';
    case UnitType::desert_eagle: return 'E';
    case UnitType::sandstorm: return 'S';
    default: return '?';
    }
}

int UnitManager::Unit::get_color() const {
    switch (type) {
    case UnitType::harvester:  return COLOR_OTHER;
    case UnitType::fremen:     return COLOR_ART_LADIES;
    case UnitType::soldier:    return COLOR_ART_LADIES;
    case UnitType::fighter:    return COLOR_HARKONNEN;
    case UnitType::heavy_tank: return COLOR_HARKONNEN;
    case UnitType::sandworm:   return COLOR_SANDWORM;
    case UnitType::desert_eagle: return COLOR_OTHER;
    case UnitType::sandstorm:  return COLOR_OTHER;
    default: return COLOR_OTHER;
    }
}

void UnitManager::Unit::print_info() const {
    std::cout << "Unit Type: " << static_cast<int>(type)
        << ", Cost: " << build_cost
        << ", Population: " << population
        << ", Speed: " << speed
        << ", Attack: " << attack_power
        << ", Health: " << health
        << ", Sight: " << sight_range
        << ", Position: (" << pos.row << ", " << pos.column << ")\n";
}

Position UnitManager::Unit::get_position() const {
    return pos;
}

UnitType UnitManager::Unit::get_type() const {
    return type;
}

void UnitManager::Unit::move(Direction d) {
    pos = ::move(pos, d);
}

void UnitManager::Unit::update(std::chrono::milliseconds current_time, Map& map) {
    if (type == UnitType::desert_eagle) {
        if (current_time - last_move_time >= std::chrono::milliseconds(speed)) {
            // 랜덤한 방향 선택 전에 현재 위치 확인
            Position new_pos;
            Direction dir;
            bool valid_move = false;

            // 유효한 이동이 찾아질 때까지 시도
            for (int attempts = 0; attempts < 4 && !valid_move; ++attempts) {
                dir = static_cast<Direction>(rand() % 4 + 1);
                new_pos = ::move(pos, dir);

                // 맵 경계 체크
                if (new_pos.row >= 1 && new_pos.row < MAP_HEIGHT - 2 &&
                    new_pos.column >= 1 && new_pos.column < MAP_WIDTH - 2) {
                    valid_move = true;
                }
            }

            // 유효한 이동이 가능할 때만 위치 업데이트
            if (valid_move) {
                pos = new_pos;
            }

            last_move_time = current_time;
        }
    }
    else if (type == UnitType::sandstorm) {
        if (current_time - last_move_time >= std::chrono::milliseconds(speed)) {
            // 이전 위치의 정보 초기화
            Position old_pos = pos;

            // 새로운 위치로 이동
            Direction dir = static_cast<Direction>(rand() % 4 + 1);
            Position new_pos = ::move(pos, dir);

            // 맵 경계 체크
            if (new_pos.row >= 0 && new_pos.row < MAP_HEIGHT - SANDSTORM_SIZE &&
                new_pos.column >= 0 && new_pos.column < MAP_WIDTH - SANDSTORM_SIZE) {
                pos = new_pos;
            }

            // 새 위치에서 데미지 체크
            check_and_damage_area(map, pos);
            last_move_time = current_time;
        }

        // 10초 후 소멸
        if (current_time - creation_time >= std::chrono::milliseconds(10000)) {
            map.add_system_message("모래 폭풍이 소멸되었습니다.");
            map.remove_unit(this);
        }
    }
    else if (type == UnitType::sandworm) {
        if (current_time - last_move_time >= std::chrono::milliseconds(speed)) {
            Position target_pos = map.find_nearest_unit(pos, UnitType::sandworm);

            if (target_pos != pos) {
                move_towards(target_pos);
            }

            // 현재 위치의 유닛 확인
            Unit* target_unit = map.get_unit_at(pos);
            if (target_unit && target_unit->get_type() != UnitType::sandworm
                && target_unit->get_type() != UnitType::heavy_tank) {
                consume_unit(*target_unit);
                // 시스템 메시지 추가
                std::string unit_type;
                switch (target_unit->get_type()) {
                case UnitType::harvester: unit_type = "하베스터"; break;
                case UnitType::fremen: unit_type = "프레멘"; break;
                case UnitType::soldier: unit_type = "보병"; break;
                case UnitType::fighter: unit_type = "전사"; break;
                default: unit_type = "유닛"; break;
                }
                map.add_system_message("샌드웜이 " + unit_type + "을(를) 잡아먹었습니다!");
                map.remove_unit(target_unit);
            }

            // 스파이스 생성 로직...
            last_move_time = current_time;
        }
    }
}

void UnitManager::Unit::move_towards(const Position& target) {
    int dx = target.column - pos.column;
    int dy = target.row - pos.row;

    Direction dir;
    if (abs(dx) > abs(dy)) {
        dir = dx > 0 ? Direction::Right : Direction::Left;
    }
    else {
        dir = dy > 0 ? Direction::Down : Direction::Up;
    }

    pos = ::move(pos, dir);
}

void UnitManager::Unit::consume_unit(Unit& target_unit) {
    length += 1;
}

void UnitManager::Unit::excrete(Map& map) {
    if (length > 1) {
        length -= 1;
        map.set_terrain(pos, TerrainType::Spice);
    }
}

bool UnitManager::Unit::is_air_unit() const {
    return type == UnitType::desert_eagle || type == UnitType::sandstorm;
}

// UnitManager 클래스 구현
void UnitManager::add_unit(std::unique_ptr<Unit> unit) {
    units.push_back(std::move(unit));
}

const std::vector<std::unique_ptr<UnitManager::Unit>>& UnitManager::get_units() const {
    return units;
}

UnitManager::Unit* UnitManager::get_unit_at(const Position& pos) const {
    for (const auto& unit : units) {
        if (unit->get_position() == pos) {
            return unit.get();
        }
    }
    return nullptr;
}

UnitManager::Unit* UnitManager::get_unit_at(const Position& pos) {
    for (auto& unit : units) {
        if (unit->get_position() == pos) {
            return unit.get();
        }
    }
    return nullptr;
}

void UnitManager::remove_unit(Unit* unit) {
    units.erase(std::remove_if(units.begin(), units.end(),
        [unit](const std::unique_ptr<Unit>& u) {
            return u.get() == unit;
        }), units.end());
}

void UnitManager::update_all_units(std::chrono::milliseconds current_time, Map& map) {
    for (auto& unit : units) {
        unit->update(current_time, map);
    }
}