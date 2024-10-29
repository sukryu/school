#include "terrain_manager.hpp"

TerrainManager::TerrainManager(int width, int height)
    : width(width), height(height),
    terrain_map(height, std::vector<Terrain>(width, Terrain(TerrainType::Desert))) {}

const Terrain& TerrainManager::get_terrain(const Position& pos) const {
    static Terrain empty_terrain(TerrainType::Empty);
    if (pos.row >= 0 && pos.row < height && pos.column >= 0 && pos.column < width) {
        return terrain_map[pos.row][pos.column];
    }
    return empty_terrain;
}

void TerrainManager::set_terrain(const Position& pos, TerrainType type) {
    if (pos.row >= 0 && pos.row < height && pos.column >= 0 && pos.column < width) {
        terrain_map[pos.row][pos.column] = Terrain(type);
    }
}