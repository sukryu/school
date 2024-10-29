#include "building_manager.hpp"

void BuildingManager::add_building(std::unique_ptr<Building> building) {
    buildings.push_back(std::move(building));
}

const std::vector<std::unique_ptr<Building>>& BuildingManager::get_buildings() const {
    return buildings;
}

const Building* BuildingManager::get_building_at(const Position& pos) const {
    for (const auto& building : buildings) {
        if (building->contains(pos)) {
            return building.get();
        }
    }
    return nullptr;
}