#include "../include/managers/terrain_manager.hpp"
#include "../include/utils/constants.hpp"
#include <iostream>

namespace dune {
    namespace managers {

        // Terrain 클래스 구현
        TerrainManager::Terrain::Terrain(types::TerrainType type)
            : type(type) {}

        wchar_t TerrainManager::Terrain::get_representation() const {
            switch (type) {
            case types::TerrainType::Desert: return L' ';
            case types::TerrainType::Plate:  return L'P';
            case types::TerrainType::Rock:   return L'R';
            case types::TerrainType::Spice:  return L'5';
            case types::TerrainType::Empty:  return L'#';
            default: return L'?';
            }
        }

        int TerrainManager::Terrain::get_color() const {
            switch (type) {
            case types::TerrainType::Desert: return constants::color::DESERT;
            case types::TerrainType::Plate:  return constants::color::PLATE;
            case types::TerrainType::Rock:   return constants::color::OTHER;
            case types::TerrainType::Spice:  return constants::color::SPICE;
            case types::TerrainType::Empty:  return constants::color::DEFAULT;
            default: return constants::color::DEFAULT;
            }
        }

        void TerrainManager::Terrain::print_info() const {
            std::wcout << L"Terrain Type: ";
            switch (type) {
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

        bool TerrainManager::Terrain::is_buildable() const {
            switch (type) {
            case types::TerrainType::Desert: return true;
            case types::TerrainType::Plate:  return true;
            case types::TerrainType::Rock:   return false;
            case types::TerrainType::Spice:  return false;
            case types::TerrainType::Empty:  return false;
            default: return false;
            }
        }

        bool TerrainManager::Terrain::is_walkable() const {
            switch (type) {
            case types::TerrainType::Desert: return true;
            case types::TerrainType::Plate:  return true;
            case types::TerrainType::Rock:   return false;
            case types::TerrainType::Spice:  return true;
            case types::TerrainType::Empty:  return false;
            default: return false;
            }
        }

        bool TerrainManager::Terrain::can_harvest_spice() const {
            return type == types::TerrainType::Spice;
        }

        // TerrainManager 클래스 구현
        TerrainManager::TerrainManager(int width, int height)
            : width(width)
            , height(height)
            , terrain_map(height, std::vector<Terrain>(width, Terrain(types::TerrainType::Desert))) {}

        const TerrainManager::Terrain& TerrainManager::get_terrain(const types::Position& pos) const {
            static Terrain empty_terrain(types::TerrainType::Empty);
            if (is_valid_position(pos)) {
                return terrain_map[pos.row][pos.column];
            }
            return empty_terrain;
        }

        void TerrainManager::set_terrain(const types::Position& pos, types::TerrainType type) {
            if (is_valid_position(pos)) {
                terrain_map[pos.row][pos.column] = Terrain(type);
            }
        }

        bool TerrainManager::is_valid_position(const types::Position& pos) const {
            return pos.row >= 0 && pos.row < height &&
                pos.column >= 0 && pos.column < width;
        }

    } // namespace managers
} // namespace dune