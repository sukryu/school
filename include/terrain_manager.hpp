#pragma once
#include "common.hpp"
#include <vector>

class TerrainManager {
public:
    class Terrain {
    public:
        Terrain(TerrainType type = TerrainType::Desert);

        char get_representation() const;
        int get_color() const;
        bool is_buildable() const;
        bool is_walkable() const;
        bool can_harvest_spice() const;
        TerrainType get_type() const;

    private:
        TerrainType type;
    };

    TerrainManager(int width, int height);
    const Terrain& get_terrain(const Position& pos) const;
    void set_terrain(const Position& pos, TerrainType type);

private:
    int width;
    int height;
    std::vector<std::vector<Terrain>> terrain_map;
};