#pragma once

#include "common.hpp"
#include <vector>
#include <memory>

class BuildingManager {
public:
    void add_building(std::unique_ptr<Building> building);
    const std::vector<std::unique_ptr<Building>>& get_buildings() const;
    const Building* get_building_at(const Position& pos) const;

private:
    std::vector<std::unique_ptr<Building>> buildings;
};