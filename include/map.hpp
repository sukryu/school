#pragma once

#include "common.hpp"
#include <vector>
#include <memory>
#include <optional>

class Map {
public:
    Map(int width, int height);
    void add_building(std::unique_ptr<Building> building);
    void add_unit(std::unique_ptr<Unit> unit);
    void update(std::chrono::milliseconds current_time);

    // 지형 관련 함수들
    void set_terrain(const Position& pos, TerrainType type);
    const Terrain& get_terrain(const Position& pos) const;
    bool is_buildable(const Position& pos) const;
    bool is_walkable(const Position& pos) const;
    bool can_harvest_spice(const Position& pos) const;

    // 특정 좌표에 있는 건물을 반환 (없으면 nullptr 반환)
    const Building* get_building_at(const Position& pos) const {
        for (const auto& building : buildings) {
            if (building->contains(pos)) {
                return building.get();
            }
        }
        return nullptr;
    }

    // 특정 좌표에 있는 유닛을 반환 (없으면 nullptr 반환)
    const Unit* get_unit_at(const Position& pos) const {
        for (const auto& unit : units) {
            if (unit->get_position() == pos) {
                return unit.get();
            }
        }
        return nullptr;
    }

    std::optional<char> get_tile(int layer, const Position& pos) const;
    void set_tile(int layer, const Position& pos, char tile);
    int get_width() const { return width; }
    int get_height() const { return height; }

private:
    int width;
    int height;
    std::vector<std::vector<Terrain>> terrain_layer;
    std::vector<std::vector<std::vector<char>>> tiles;
    std::vector<std::unique_ptr<Building>> buildings;
    std::vector<std::unique_ptr<Unit>> units;
};
