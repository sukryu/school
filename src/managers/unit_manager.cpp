#include "managers/unit_manager.hpp"
#include "utils/utils.hpp"
#include <iostream>

namespace dune {
    namespace managers {

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
            , last_move_time(0) {
                switch(type) {
                case types::UnitType::Harvester:
                    build_cost = 5;
                    population = 5;
                    speed = constants::HARVESTER_SPEED;
                    attack_power = 0;
                    health = 70;
                    sight_range = 0;
                    break;
                default:
                    build_cost = 5;
                    population = 5;
                    speed = 0;
                    attack_power = 0;
                    health = 70;
                    sight_range = 0;
                    break;
                }
            }

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

        wchar_t UnitManager::Unit::get_representation() const {
            switch (type) {
            case types::UnitType::Harvester: return L'H';
            case types::UnitType::Fremen: return L'M';
            case types::UnitType::Soldier: return L'I';
            case types::UnitType::Fighter: return L'J';
            case types::UnitType::HeavyTank: return L'T';
            case types::UnitType::Sandworm: return L'W';
            default: return L'?';
            }
        }

        int UnitManager::Unit::get_color() const {
            switch (type) {
            case types::UnitType::Harvester:  return constants::color::OTHER;
            case types::UnitType::Fremen:     return constants::color::ART_LADIES;
            case types::UnitType::Soldier:    return constants::color::ART_LADIES;
            case types::UnitType::Fighter:    return constants::color::HARKONNEN;
            case types::UnitType::HeavyTank:  return constants::color::HARKONNEN;
            case types::UnitType::Sandworm:   return constants::color::SANDWORM;
            default: return constants::color::OTHER;
            }
        }

        void UnitManager::Unit::print_info() const {
            std::wcout << L"Unit Type: " << static_cast<int>(type)
                << L", Cost: " << build_cost
                << L", Population: " << population
                << L", Speed: " << speed
                << L", Attack: " << attack_power
                << L", Health: " << health
                << L", Sight: " << sight_range
                << L", Position: (" << pos.row << L", " << pos.column << L")\n";
        }

        void UnitManager::Unit::move(types::Direction d) {
            pos = utils::move(pos, d);
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

    } // namespace managers
} // namespace dune