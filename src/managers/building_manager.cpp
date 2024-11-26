#include "building_manager.hpp"
#include "terrain_manager.hpp"
#include "../utils/constants.hpp"
#include "../utils/utils.hpp"
#include <iostream>

namespace dune {
    namespace managers {

        // Building 클래스 구현

        BuildingManager::Building::Building(types::Camp type, const std::wstring& name, const std::wstring& description,
                                            int buildCost, types::Position position, int width, int height, types::UnitType producedUnit)
            : type_(type)
            , name_(name)
            , description_(description)
            , buildingCost_(buildCost)
            , position_(position)
            , width_(width)
            , height_(height)
            , producedUnit_(producedUnit)
            , health_(constants::DEFAULT_HEALTH) {}

        wchar_t BuildingManager::Building::getRepresentation() const {
            if (name_ == L"Base") return L'B';
            if (name_ == L"Plate") return L'P';
            if (name_ == L"Dormitory") return L'D';
            if (name_ == L"Garage") return L'G';
            if (name_ == L"Barracks") return L'K';
            if (name_ == L"Shelter") return L'S';
            if (name_ == L"Arena") return L'A';
            if (name_ == L"Factory") return L'F';
            return L'?';
        }

        int BuildingManager::Building::getColor() const {
            switch (type_) {
            case types::Camp::Common:     return constants::color::ART_LADIES;
            case types::Camp::ArtLadies:  return constants::color::ART_LADIES;
            case types::Camp::Harkonnen:  return constants::color::HARKONNEN;
            default:                      return constants::color::OTHER;
            }
        }

        void BuildingManager::Building::printInfo() const {
            std::wcout << L"Building: " << name_
                << L", Description: " << description_
                << L", Cost: " << buildingCost_
                << L", Position: (" << position_.row << L", " << position_.column << L")"
                << L", Size: " << width_ << L"x" << height_
                << L", Health: " << health_ << std::endl;
        }

        void BuildingManager::Building::move(types::Direction direction) {
            position_ = dune::utils::move(position_, direction);
        }

        bool BuildingManager::Building::contains(const types::Position& position) const {
            return position.row >= position_.row && position.row < position_.row + height_ &&
                   position.column >= position_.column && position.column < position_.column + width_;
        }

        void BuildingManager::Building::takeDamage(int damage) {
            health_ = std::max(0, health_ - damage);
        }

        bool BuildingManager::Building::isPlaceable(const types::Position& position, const TerrainManager& terrainManager) const {
            for (int i = 0; i < height_; ++i) {
                for (int j = 0; j < width_; ++j) {
                    types::Position checkPos = { position.row + i, position.column + j };
                    // 맵 범위 체크
                    if (checkPos.row < 0 || checkPos.row >= constants::MAP_HEIGHT ||
                        checkPos.column < 0 || checkPos.column >= constants::MAP_WIDTH) {
                        return false;
                    }
                    // 지형 체크
                    if (!terrainManager.getTerrain(checkPos).isBuildable()) {
                        return false;
                    }
                }
            }
            return true;
        }

        // BuildingManager 클래스 구현

        void BuildingManager::addBuilding(std::unique_ptr<Building> building) {
            buildings_.push_back(std::move(building));
        }

        BuildingManager::Building* BuildingManager::getBuildingAt(const types::Position& position) {
            for (auto& building : buildings_) {
                if (building->contains(position)) {
                    return building.get();
                }
            }
            return nullptr;
        }

        const BuildingManager::Building* BuildingManager::getBuildingAt(const types::Position& position) const {
            for (const auto& building : buildings_) {
                if (building->contains(position)) {
                    return building.get();
                }
            }
            return nullptr;
        }

        void BuildingManager::removeBuilding(Building* building) {
            buildings_.erase(
                std::remove_if(buildings_.begin(), buildings_.end(),
                    [building](const std::unique_ptr<Building>& b) {
                        return b.get() == building;
                    }),
                buildings_.end()
            );
        }

        const std::vector<std::unique_ptr<BuildingManager::Building>>& BuildingManager::getBuildings() const {
            return buildings_;
        }

        void BuildingManager::removeDestroyedBuildings() {
            buildings_.erase(
                std::remove_if(buildings_.begin(), buildings_.end(),
                    [](const std::unique_ptr<Building>& building) {
                        return building->isDestroyed();
                    }),
                buildings_.end()
            );
        }

    } // namespace managers
} // namespace dune
