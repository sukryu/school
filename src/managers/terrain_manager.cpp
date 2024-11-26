#include "terrain_manager.hpp"
#include "../utils/constants.hpp"
#include <iostream>

namespace dune {
    namespace managers {

        // Terrain 클래스 구현

        Terrain::Terrain(types::TerrainType type, const types::Position& position)
            : type_(type), position_(position) {}

        wchar_t Terrain::getRepresentation() const {
            switch (type_) {
            case types::TerrainType::Desert: return L' ';
            case types::TerrainType::Plate:  return L'P';
            case types::TerrainType::Rock:   return L'R';
            case types::TerrainType::Spice:  return L'5';
            case types::TerrainType::Empty:  return L'#';
            default: return L'?';
            }
        }

        int Terrain::getColor() const {
            switch (type_) {
            case types::TerrainType::Desert: return constants::color::DESERT;
            case types::TerrainType::Plate:  return constants::color::PLATE;
            case types::TerrainType::Rock:   return constants::color::OTHER;
            case types::TerrainType::Spice:  return constants::color::SPICE;
            case types::TerrainType::Empty:  return constants::color::DEFAULT;
            default: return constants::color::DEFAULT;
            }
        }

        void Terrain::printInfo() const {
            std::wcout << L"Terrain Type: ";
            switch (type_) {
            case types::TerrainType::Desert:
                std::wcout << L"Desert\n";
                break;
            case types::TerrainType::Plate:
                std::wcout << L"Plate\n";
                break;
            case types::TerrainType::Rock:
                std::wcout << L"Rock\n";
                break;
            case types::TerrainType::Spice:
                std::wcout << L"Spice\n";
                break;
            default:
                std::wcout << L"Unknown\n";
            }
        }

        bool Terrain::isBuildable() const {
            switch (type_) {
            case types::TerrainType::Desert: return true;
            case types::TerrainType::Plate:  return true;
            case types::TerrainType::Rock:   return false;
            case types::TerrainType::Spice:  return false;
            case types::TerrainType::Empty:  return false;
            default: return false;
            }
        }

        bool Terrain::isWalkable() const {
            switch (type_) {
            case types::TerrainType::Desert: return true;
            case types::TerrainType::Plate:  return true;
            case types::TerrainType::Rock:   return false;
            case types::TerrainType::Spice:  return true;
            case types::TerrainType::Empty:  return false;
            default: return false;
            }
        }

        bool Terrain::canHarvestSpice() const {
            return type_ == types::TerrainType::Spice;
        }

        // TerrainManager 클래스 구현

        TerrainManager::TerrainManager(int width, int height)
            : width_(width), height_(height),
              terrainMap_(height, std::vector<Terrain>(width, Terrain(types::TerrainType::Desert))) {
            // 각 지형의 위치 설정
            for (int row = 0; row < height_; ++row) {
                for (int col = 0; col < width_; ++col) {
                    types::Position position{ row, col };
                    terrainMap_[row][col] = Terrain(types::TerrainType::Desert, position);
                }
            }
        }

        const Terrain& TerrainManager::getTerrain(const types::Position& position) const {
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
