#pragma once
#include "common.hpp"
#include <vector>
#include <memory>
#include <cstring>

class BuildingManager {
public:
    class Building {
    public:
        Building(Camp type, const char* name, const char* description, int build_cost, Position pos, int width, int height, UnitType produced_unit);

        char get_representation() const;
        int get_color() const;
        void print_info() const;
        bool contains(const Position& p) const;
        Camp get_type() const;
        const char* get_name() const;
        const int get_height() const;
        const int get_width() const;
        Position get_position() const;
        void move(Direction d);
        void take_damage(int damage);
        bool is_destroyed() const;

    private:
        Camp type;
        const char* name;
        const char* description;
        int building_cost;
        Position pos;
        int width;
        int height;
        UnitType produced_unit;
        int health = 100;
    };

    void add_building(std::unique_ptr<Building> building);
    const std::vector<std::unique_ptr<Building>>& get_buildings() const;
    Building* get_building_at(const Position& pos) const;
    Building* get_building_at(const Position& pos);
    void remove_destroyed_buildings();

private:
    std::vector<std::unique_ptr<Building>> buildings;
};