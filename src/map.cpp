#include "map.hpp"

Map::Map(int width, int height)
    : width(width), height(height),
    terrain_manager(width, height) {}

void Map::update(std::chrono::milliseconds current_time) {
    // 유닛 및 건물 업데이트 로직 추가 가능
}

const TerrainManager& Map::get_terrain_manager() const {
    return terrain_manager;
}

const UnitManager& Map::get_unit_manager() const {
    return unit_manager;
}

const BuildingManager& Map::get_building_manager() const {
    return building_manager;
}

void Map::add_unit(std::unique_ptr<Unit> unit) {
    unit_manager.add_unit(std::move(unit));
}

void Map::add_building(std::unique_ptr<Building> building) {
    building_manager.add_building(std::move(building));
}

void Map::set_terrain(const Position& pos, TerrainType type) {
    terrain_manager.set_terrain(pos, type);
}

const Unit* Map::get_unit_at(const Position& pos) const {
    return unit_manager.get_unit_at(pos);
}

const Building* Map::get_building_at(const Position& pos) const {
    return building_manager.get_building_at(pos);
}