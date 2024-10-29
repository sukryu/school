#pragma once

#include "common.hpp"
#include <vector>

class TerrainManager {
public:
    TerrainManager(int width, int height);
    const Terrain& get_terrain(const Position& pos) const;
    void set_terrain(const Position& pos, TerrainType type);

private:
    int width;
    int height;
    std::vector<std::vector<Terrain>> terrain_map;
};