#include "../include/utils/constants.hpp"
#include "../include/utils/utils.hpp"
#include "../include/core/map.hpp"
#include <iostream>

namespace dune {
    namespace managers {

        // Unit 클래스 구현
        UnitManager::Unit::Unit(types::UnitType type, int build_cost, int population, int speed,
            int attack_power, int health, int sight_range, types::Position pos)
            : type(type)
            , build_cost(build_cost)
            , population(population)
            , speed(speed)
            , attack_power(attack_power)
            , health(health)
            , sight_range(sight_range)
            , pos(pos)
            , length(1)
            , last_move_time(0)
            , creation_time(0) {}

        UnitManager::Unit::Unit(types::UnitType type, types::Position pos)
            : type(type)
            , pos(pos) {
            switch (type) {
            case types::UnitType::Sandworm:
                build_cost = 0;
                population = 0;
                speed = constants::SANDWORM_SPEED;
                attack_power = 9999;
                health = constants::DEFAULT_HEALTH;
                sight_range = 999;
                length = 1;
                break;
            case types::UnitType::DesertEagle:
                build_cost = 0;
                population = 0;
                speed = 200;
                attack_power = 0;
                health = 10;
                sight_range = 5;
                break;
            case types::UnitType::Sandstorm:
                build_cost = 0;
                population = 0;
                speed = 100;
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

        char UnitManager::Unit::get_representation() const {
            switch (type) {
            case types::UnitType::Harvester: return 'H';
            case types::UnitType::Fremen: return 'M';
            case types::UnitType::Soldier: return 'I';
            case types::UnitType::Fighter: return 'J';
            case types::UnitType::HeavyTank: return 'T';
            case types::UnitType::Sandworm: return 'W';
            case types::UnitType::DesertEagle: return 'E';
            case types::UnitType::Sandstorm: return 'S';
            default: return '?';
            }
        }

        int UnitManager::Unit::get_color() const {
            switch (type) {
            case types::UnitType::Harvester:  return constants::color::OTHER;
            case types::UnitType::Fremen:     return constants::color::ART_LADIES;
            case types::UnitType::Soldier:    return constants::color::ART_LADIES;
            case types::UnitType::Fighter:    return constants::color::HARKONNEN;
            case types::UnitType::HeavyTank: return constants::color::HARKONNEN;
            case types::UnitType::Sandworm:   return constants::color::SANDWORM;
            case types::UnitType::DesertEagle: return constants::color::OTHER;
            case types::UnitType::Sandstorm:  return constants::color::OTHER;
            default: return constants::color::OTHER;
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

        void UnitManager::Unit::move(types::Direction d) {
            pos = dune::utils::move(pos, d);
        }

        void UnitManager::Unit::check_and_damage_area(core::Map& map, const types::Position& pos) {
            for (int i = 0; i < SANDSTORM_SIZE; ++i) {
                for (int j = 0; j < SANDSTORM_SIZE; ++j) {
                    types::Position check_pos = { pos.row + i, pos.column + j };

                    // 유닛 제거
                    Unit* unit = map.get_entity_at<Unit>(check_pos);
                    if (unit && unit != this) {
                        map.add_system_message("모래 폭풍이 유닛을 파괴했습니다!");
                        map.remove_unit(unit);
                    }

                    // 건물 데미지
                    auto* building = map.get_entity_at<BuildingManager::Building>(check_pos);
                    if (building) {
                        building->take_damage(constants::SANDSTORM_DAMAGE);
                        map.add_system_message("모래 폭풍이 건물에 데미지를 입혔습니다!");
                    }
                }
            }
        }

        void UnitManager::Unit::update(std::chrono::milliseconds current_time, core::Map& map) {
            if (type == types::UnitType::Sandworm) {
                if (current_time - last_move_time >= std::chrono::milliseconds(speed)) {
                    types::Position target_pos = map.find_nearest_unit(pos, types::UnitType::Sandworm);

                    if (target_pos != pos) {
                        move_towards(target_pos);
                    }

                    Unit* target_unit = map.get_entity_at<Unit>(pos);
                    if (target_unit && target_unit->get_type() != types::UnitType::Sandworm
                        && target_unit->get_type() != types::UnitType::HeavyTank) {
                        consume_unit(*target_unit);

                        std::string unit_type;
                        switch (target_unit->get_type()) {
                        case types::UnitType::Harvester: unit_type = "하베스터"; break;
                        case types::UnitType::Fremen: unit_type = "프레멘"; break;
                        case types::UnitType::Soldier: unit_type = "보병"; break;
                        case types::UnitType::Fighter: unit_type = "전사"; break;
                        default: unit_type = "유닛"; break;
                        }
                        map.add_system_message("샌드웜이 " + unit_type + "을(를) 잡아먹었습니다!");
                        map.remove_unit(target_unit);
                    }

                    last_move_time = current_time;
                }
            }
        }

        void UnitManager::Unit::move_towards(const types::Position& target) {
            int dx = target.column - pos.column;
            int dy = target.row - pos.row;

            types::Direction dir;
            if (abs(dx) > abs(dy)) {
                dir = dx > 0 ? types::Direction::Right : types::Direction::Left;
            }
            else {
                dir = dy > 0 ? types::Direction::Down : types::Direction::Up;
            }

            move(dir);
        }

        void UnitManager::Unit::consume_unit(Unit& target_unit) {
            length += 1;
        }

        void UnitManager::Unit::excrete(core::Map& map) {
            if (length > 1) {
                length -= 1;
                map.set_terrain(pos, types::TerrainType::Spice);
            }
        }

        bool UnitManager::Unit::is_air_unit() const {
            return type == types::UnitType::DesertEagle ||
                type == types::UnitType::Sandstorm;
        }

        // UnitManager 클래스 구현
        void UnitManager::add_unit(std::unique_ptr<Unit> unit) {
            units.push_back(std::move(unit));
        }

        UnitManager::Unit* UnitManager::get_unit_at(const types::Position& pos) {
            for (auto& unit : units) {
                if (unit->get_position() == pos) {
                    return unit.get();
                }
            }
            return nullptr;
        }

        const UnitManager::Unit* UnitManager::get_unit_at(const types::Position& pos) const {
            for (const auto& unit : units) {
                if (unit->get_position() == pos) {
                    return unit.get();
                }
            }
            return nullptr;
        }

        void UnitManager::remove_unit(Unit* unit) {
            units.erase(
                std::remove_if(units.begin(), units.end(),
                    [unit](const std::unique_ptr<Unit>& u) {
                        return u.get() == unit;
                    }),
                units.end()
            );
        }

        const std::vector<std::unique_ptr<UnitManager::Unit>>& UnitManager::get_units() const {
            return units;
        }

        void UnitManager::update_all_units(std::chrono::milliseconds current_time, core::Map& map) {
            for (auto& unit : units) {
                unit->update(current_time, map);
            }
        }

    } // namespace managers
} // namespace dune