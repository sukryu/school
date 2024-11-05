#pragma once
#include "common.hpp"
#include "unit_manager.hpp"
#include "building_manager.hpp"
#include "terrain_manager.hpp"
#include "message_window.hpp"

class Map {
public:
    using Unit = UnitManager::Unit;
    using Building = BuildingManager::Building;

    Map(int width, int height);
    // Manager 접근자
    const TerrainManager& get_terrain_manager() const;
    const UnitManager& get_unit_manager() const;
    UnitManager& get_unit_manager();
    const BuildingManager& get_building_manager() const;

    // 객체 추가 함수
    void add_unit(std::unique_ptr<Unit> unit);
    void add_building(std::unique_ptr<Building> building);
    void set_terrain(const Position& pos, TerrainType type);

    // 객체 조회 함수
    const Unit* get_unit_at(const Position& pos) const;
    Unit* get_unit_at(const Position& pos);
    const Building* get_building_at(const Position& pos) const;
    Building* get_building_at(const Position& pos);

    // 유닛 관련 함수
    Position find_nearest_unit(const Position& from_pos, UnitType exclude_type);
    void remove_unit(Unit* unit);

    // 건물 관련 함수
    void damage_building_at(const Position& pos, int damage);
    void remove_destroyed_buildings();

    void update(std::chrono::milliseconds current_time);
    void spawn_sandstorm();

    void add_system_message(const std::string& message) {
        system_message.add_message(message);
    }

private:
    int width;
    int height;
    TerrainManager terrain_manager;
    UnitManager unit_manager;
    BuildingManager building_manager;
    MessageWindow system_message{ 0, MAP_HEIGHT + RESOURCE_HEIGHT, MAP_WIDTH, SYSTEM_MESSAGE_HEIGHT };
    std::chrono::milliseconds last_sandstorm_time{ 0 };
    static constexpr std::chrono::milliseconds SANDSTORM_INTERVAL{ 3000 };
};