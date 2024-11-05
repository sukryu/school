#include "terrain_manager.hpp"

// Terrain 클래스 구현
TerrainManager::Terrain::Terrain(TerrainType type) : type(type) {}

char TerrainManager::Terrain::get_representation() const {
    switch (type) {
    case TerrainType::Desert: return ' ';
    case TerrainType::Plate:  return 'P';
    case TerrainType::Rock:   return 'R';
    case TerrainType::Spice:  return '5';
    case TerrainType::Empty:  return '#';
    default: return '?';
    }
}

int TerrainManager::Terrain::get_color() const {
    switch (type) {
    case TerrainType::Desert: return COLOR_DEFAULT;
    case TerrainType::Plate:  return COLOR_PLATE;
    case TerrainType::Rock:   return COLOR_OTHER;
    case TerrainType::Spice:  return COLOR_SPICE;
    case TerrainType::Empty:  return COLOR_DEFAULT;
    default: return COLOR_DEFAULT;
    }
}

bool TerrainManager::Terrain::is_buildable() const {
    switch (type) {
    case TerrainType::Desert: return true;
    case TerrainType::Plate:  return true;
    case TerrainType::Rock:   return false;
    case TerrainType::Spice:  return false;
    case TerrainType::Empty:  return false;
    default: return false;
    }
}

bool TerrainManager::Terrain::is_walkable() const {
    switch (type) {
    case TerrainType::Desert: return true;
    case TerrainType::Plate:  return true;
    case TerrainType::Rock:   return false;
    case TerrainType::Spice:  return true;
    case TerrainType::Empty:  return false;
    default: return false;
    }
}

bool TerrainManager::Terrain::can_harvest_spice() const {
    return type == TerrainType::Spice;
}

TerrainType TerrainManager::Terrain::get_type() const {
    return type;
}

// TerrainManager 클래스 구현
TerrainManager::TerrainManager(int width, int height)
    : width(width), height(height),
    terrain_map(height, std::vector<Terrain>(width, Terrain(TerrainType::Desert))) {}

const TerrainManager::Terrain& TerrainManager::get_terrain(const Position& pos) const {
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