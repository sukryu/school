#include "building_manager.hpp"
#include <iostream>

// Building 클래스 구현
BuildingManager::Building::Building(Camp type, const char* name, const char* description,
    int build_cost, Position pos, int width, int height, UnitType produced_unit)
    : type(type), name(name), description(description), building_cost(build_cost),
    pos(pos), width(width), height(height), produced_unit(produced_unit), health(100) {
}

char BuildingManager::Building::get_representation() const {
    if (strcmp(name, "Base") == 0) return 'B';
    if (strcmp(name, "Plate") == 0) return 'P';
    if (strcmp(name, "Dormitory") == 0) return 'D';
    if (strcmp(name, "Garage") == 0) return 'G';
    if (strcmp(name, "Barracks") == 0) return 'K';
    if (strcmp(name, "Shelter") == 0) return 'S';
    if (strcmp(name, "Arena") == 0) return 'A';
    if (strcmp(name, "Factory") == 0) return 'F';
    return '?';
}

int BuildingManager::Building::get_color() const {
    switch (type) {
    case Camp::common:      return COLOR_OTHER;
    case Camp::art_ladies:  return COLOR_ART_LADIES;
    case Camp::harkonnen:   return COLOR_HARKONNEN;
    default: return COLOR_OTHER;
    }
}

void BuildingManager::Building::print_info() const {
    std::cout << "Building: " << name
        << ", Description: " << description
        << ", Cost: " << building_cost
        << ", Position: (" << pos.row << ", " << pos.column << ")"
        << ", Size: " << width << "x" << height
        << ", Health: " << health
        << ", Produced Unit: " << static_cast<int>(produced_unit) << std::endl;
}

bool BuildingManager::Building::contains(const Position& p) const {
    return p.row >= pos.row && p.row < pos.row + height &&
        p.column >= pos.column && p.column < pos.column + width;
}

Camp BuildingManager::Building::get_type() const {
    return type;
}

const char* BuildingManager::Building::get_name() const {
    return name;
}

const int BuildingManager::Building::get_height() const {
    return height;
}

const int BuildingManager::Building::get_width() const {
    return width;
}

Position BuildingManager::Building::get_position() const {
    return pos;
}

void BuildingManager::Building::move(Direction d) {
    pos = ::move(pos, d);
}

void BuildingManager::Building::take_damage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool BuildingManager::Building::is_destroyed() const {
    return health <= 0;
}

// BuildingManager 클래스 구현
void BuildingManager::add_building(std::unique_ptr<Building> building) {
    buildings.push_back(std::move(building));
}

const std::vector<std::unique_ptr<BuildingManager::Building>>& BuildingManager::get_buildings() const {
    return buildings;
}

BuildingManager::Building* BuildingManager::get_building_at(const Position& pos) const {
    for (const auto& building : buildings) {
        if (building->contains(pos)) {
            return building.get();
        }
    }
    return nullptr;
}

BuildingManager::Building* BuildingManager::get_building_at(const Position& pos) {
    for (const auto& building : buildings) {
        if (building->contains(pos)) {
            return building.get();
        }
    }
    return nullptr;
}

void BuildingManager::remove_destroyed_buildings() {
    buildings.erase(
        std::remove_if(buildings.begin(), buildings.end(),
            [](const std::unique_ptr<Building>& building) {
                return building->is_destroyed();
            }),
        buildings.end()
    );
}