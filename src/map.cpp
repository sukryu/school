#include "map.hpp"
#include <algorithm>

Map::Map(int width, int height) : width(width), height(height) {
    // 테두리를 제외한 실제 맵 영역만큼 지형 레이어 초기화
    terrain_layer.resize(height, std::vector<Terrain>(width, Terrain(TerrainType::Desert)));

    // 디스플레이용 타일 초기화
    tiles.resize(N_LAYER, std::vector<std::vector<char>>(height, std::vector<char>(width, ' ')));

    // 테두리 초기화 (#)
    for (int j = 0; j < width; j++) {
        tiles[0][0][j] = '#';
        tiles[0][height - 1][j] = '#';
    }
    for (int i = 0; i < height; i++) {
        tiles[0][i][0] = '#';
        tiles[0][i][width - 1] = '#';
    }
}

void Map::set_terrain(const Position& pos, TerrainType type) {
    // 좌표가 유효한지 확인
    if (pos.row >= 0 && pos.row < height - 2 &&
        pos.column >= 0 && pos.column < width - 2) {
        terrain_layer[pos.row][pos.column] = Terrain(type);
    }
}

const Terrain& Map::get_terrain(const Position& pos) const {
    static const Terrain default_terrain(TerrainType::Empty);
    if (pos.row >= 0 && pos.row < height - 2 &&
        pos.column >= 0 && pos.column < width - 2) {
        return terrain_layer[pos.row][pos.column];
    }
    return default_terrain;
}

bool Map::is_buildable(const Position& pos) const {
    return get_terrain(pos).is_buildable();
}

bool Map::is_walkable(const Position& pos) const {
    return get_terrain(pos).is_walkable();
}

bool Map::can_harvest_spice(const Position& pos) const {
    return get_terrain(pos).can_harvest_spice();
}

void Map::add_building(std::unique_ptr<Building> building) {
    buildings.push_back(std::move(building));
}

void Map::add_unit(std::unique_ptr<Unit> unit) {
    units.push_back(std::move(unit));
}

void Map::update(std::chrono::milliseconds current_time) {
    // Layer 1 초기화
    for (auto& row : tiles[1]) {
        std::fill(row.begin(), row.end(), ' ');
    }

    // 테두리 제외한 영역에 지형 정보 업데이트
    for (int i = 0; i < height - 2; i++) {
        for (int j = 0; j < width - 2; j++) {
            const Terrain& terrain = terrain_layer[i][j];
            if (terrain.get_type() != TerrainType::Desert) {
                // 실제 표시 위치는 테두리를 고려하여 +1
                tiles[0][i + 1][j + 1] = terrain.get_representation();
            }
        }
    }

    // 유닛 업데이트
    for (const auto& unit : units) {
        Position pos = unit->get_position();
        if (pos.row >= 0 && pos.row < height - 2 &&
            pos.column >= 0 && pos.column < width - 2) {
            tiles[1][pos.row + 1][pos.column + 1] = unit->get_representation();
        }
    }

    // 건물 업데이트
    for (const auto& building : buildings) {
        Position pos = building->get_position();
        for (int i = 0; i < building->get_height(); i++) {
            for (int j = 0; j < building->get_width(); j++) {
                if (pos.row + i >= 0 && pos.row + i < height - 2 &&
                    pos.column + j >= 0 && pos.column + j < width - 2) {
                    tiles[1][pos.row + i + 1][pos.column + j + 1] =
                        building->get_representation();
                }
            }
        }
    }
}

std::optional<char> Map::get_tile(int layer, const Position& pos) const {
    if (pos.row >= 0 && pos.row < height && pos.column >= 0 && pos.column < width) {
        return tiles[layer][pos.row][pos.column];
    }
    return std::nullopt;
}

void Map::set_tile(int layer, const Position& pos, char tile) {
    if (pos.row >= 0 && pos.row < height && pos.column >= 0 && pos.column < width) {
        tiles[layer][pos.row][pos.column] = tile;
    }
}
