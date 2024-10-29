#pragma once

#include "common.hpp"
#include "unit_manager.hpp"
#include "building_manager.hpp"
#include "terrain_manager.hpp"

class Map {
public:
    Map(int width, int height);
    void update(std::chrono::milliseconds current_time);

    const TerrainManager& get_terrain_manager() const;
    const UnitManager& get_unit_manager() const;
    const BuildingManager& get_building_manager() const;

    // 기존의 함수들
    void add_unit(std::unique_ptr<Unit> unit);
    void add_building(std::unique_ptr<Building> building);
    void set_terrain(const Position& pos, TerrainType type);

    // 유닛과 건물 조회 함수
    const Unit* get_unit_at(const Position& pos) const;
    const Building* get_building_at(const Position& pos) const;

private:
    int width;
    int height;
    TerrainManager terrain_manager;
    UnitManager unit_manager;
    BuildingManager building_manager;
};