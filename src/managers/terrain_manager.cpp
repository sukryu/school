#include "managers/terrain_manager.hpp"
#include "utils/constants.hpp"
#include <iostream>

namespace dune {
    namespace managers {
        // TerrainManager 클래스 구현

        TerrainManager::TerrainManager(int width, int height)
            : width_(width), height_(height),
            terrainMap_(height, std::vector<Terrain>(width))
        {
            for (int row = 0; row < height_; ++row) {
                for (int col = 0; col < width_; ++col) {
                    types::Position pos{ row, col };
                    terrainMap_[row][col] = Terrain(types::TerrainType::Desert, pos);
                }
            }
        }

        const TerrainManager::Terrain& TerrainManager::getTerrain(const types::Position& position) const {
            static Terrain emptyTerrain(types::TerrainType::Empty);
            if (isValidPosition(position)) {
                return terrainMap_[position.row][position.column];
            }
            return emptyTerrain;
        }

        void TerrainManager::setTerrain(const types::Position& position, types::TerrainType type) {
            if (isValidPosition(position)) {
                terrainMap_[position.row][position.column] = Terrain(type, position);
            }
        }
        
        bool TerrainManager::isValidPosition(const types::Position& position) const {
            return position.row >= 0 && position.row < height_ &&
                   position.column >= 0 && position.column < width_;
        }

    } // namespace managers
} // namespace dune
