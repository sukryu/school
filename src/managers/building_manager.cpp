#include "managers/building_manager.hpp"
#include "managers/terrain_manager.hpp"
#include "utils/constants.hpp"
#include "utils/utils.hpp"
#include <iostream>

namespace dune {
    namespace managers {
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
