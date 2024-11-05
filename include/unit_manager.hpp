#pragma once

#include "common.hpp"
#include <vector>
#include <memory>

class Map;

class UnitManager {
public:
    class Unit;
    void add_unit(std::unique_ptr<Unit> unit);
    const std::vector<std::unique_ptr<Unit>>& get_units() const;
    Unit* get_unit_at(const Position& pos) const;
    Unit* get_unit_at(const Position& pos);
    void remove_unit(Unit* unit);
    void update_all_units(std::chrono::milliseconds current_time, Map& map);

private:
    std::vector<std::unique_ptr<Unit>> units;
};

class UnitManager::Unit {
    public:
        Unit(UnitType type, int build_cost, int population, int speed, int attack_power, int health, int sight_range, Position pos);
        Unit(UnitType type, Position pos); // Sandworm & air unit용 생성자 오버로드

        static constexpr int SANDSTORM_SIZE = 2;
        std::chrono::milliseconds creation_time;
        void check_and_damage_area(Map& map, const Position& pos);

        char get_representation() const;
        int get_color() const;
        void print_info() const;
        Position get_position() const;
        UnitType get_type() const;

        void move(Direction d);
        void update(std::chrono::milliseconds current_time, Map& map);
        void move_towards(const Position& target);
        void consume_unit(Unit& target_unit);
        void excrete(Map& map);
        bool is_air_unit() const;

    private:
        UnitType type;
        int build_cost;
        int population;
        int speed;
        int attack_power;
        int health;
        int sight_range;
        Position pos;
        int length = 1; // 샌드웜 길이
        std::chrono::milliseconds last_move_time{ 0 };
        std::chrono::milliseconds sand_worm_creation_time{ 0 };
    };