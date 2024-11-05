#include "map.hpp"

Map::Map(int width, int height)
    : width(width), height(height),
    terrain_manager(width, height) {}

void Map::update(std::chrono::milliseconds current_time) {
    unit_manager.update_all_units(current_time, *this);

    if (current_time - last_sandstorm_time >= SANDSTORM_INTERVAL) {
        spawn_sandstorm();
        last_sandstorm_time = current_time;
    }
}

const TerrainManager& Map::get_terrain_manager() const {
    return terrain_manager;
}

const UnitManager& Map::get_unit_manager() const {
    return unit_manager;
}

UnitManager& Map::get_unit_manager() {
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

const Map::Unit* Map::get_unit_at(const Position& pos) const {
    return unit_manager.get_unit_at(pos);
}

Map::Unit* Map::get_unit_at(const Position& pos) {
    return unit_manager.get_unit_at(pos);
}

const Map::Building* Map::get_building_at(const Position& pos) const {
    return building_manager.get_building_at(pos);
}
Map::Building* Map::get_building_at(const Position& pos) {
    return building_manager.get_building_at(pos);
}

Position Map::find_nearest_unit(const Position& from_pos, UnitType exclude_type) {
    Position nearest_pos = from_pos;
    int min_distance = INT_MAX;

    for (const auto& unit : unit_manager.get_units()) {
        if (unit->get_type() != exclude_type) {
            Position unit_pos = unit->get_position();
            int distance = abs(unit_pos.row - from_pos.row) +
                abs(unit_pos.column - from_pos.column);
            if (distance < min_distance) {
                min_distance = distance;
                nearest_pos = unit_pos;
            }
        }
    }
    return nearest_pos;
}

void Map::remove_unit(Unit* unit) {
    unit_manager.remove_unit(unit);
}

void Map::damage_building_at(const Position& pos, int damage) {
    if (Building* building = building_manager.get_building_at(pos)) {
        building->take_damage(damage);
    }
}

void Map::remove_destroyed_buildings() {
    building_manager.remove_destroyed_buildings();
}

void Map::spawn_sandstorm() {
    // 랜덤 위치에 모래 폭풍 생성
    Position spawn_pos = {
        rand() % (MAP_HEIGHT - 2),
        rand() % (MAP_WIDTH - 2)
    };
    
    add_unit(std::make_unique<UnitManager::Unit>(
        UnitType::sandstorm,
        spawn_pos
    ));
    
    add_system_message("새로운 모래 폭풍이 발생했습니다!");
}