#include "../include/managers/building_manager.hpp"
#include "managers/terrain_manager.hpp"
#include "../include/utils/constants.hpp"
#include "../include/utils/utils.hpp"
#include <iostream>

namespace dune {
    namespace managers {

        // Building 클래스 구현
        BuildingManager::Building::Building(types::Camp type, std::wstring name, std::wstring description,
            int build_cost, types::Position pos, int width, int height, types::UnitType produced_unit)
            : type(type)
            , name(std::move(name))
            , description(std::move(description))
            , building_cost(build_cost)
            , pos(pos)
            , width(width)
            , height(height)
            , produced_unit(produced_unit)
            , health(constants::DEFAULT_HEALTH) {}

        wchar_t BuildingManager::Building::get_representation() const {
            if (name == L"Base") return L'B';
            if (name == L"Plate") return L'P';
            if (name == L"Dormitory") return L'D';
            if (name == L"Garage") return L'G';
            if (name == L"Barracks") return L'K';
            if (name == L"Shelter") return L'S';
            if (name == L"Arena") return L'A';
            if (name == L"Factory") return L'F';
            return L'?';
        }

        int BuildingManager::Building::get_color() const {
            switch (type) {
            case types::Camp::Common:      return constants::color::OTHER;
            case types::Camp::ArtLadies:  return constants::color::ART_LADIES;
            case types::Camp::Harkonnen:   return constants::color::HARKONNEN;
            default: return constants::color::OTHER;
            }
        }

        void BuildingManager::Building::print_info() const {
            std::wcout << L"Building: " << name
                << L", Description: " << description
                << L", Cost: " << building_cost
                << L", Position: (" << pos.row << L", " << pos.column << L")"
                << L", Size: " << width << L"x" << height
                << L", Health: " << health << std::endl;
        }

        void BuildingManager::Building::move(types::Direction d) {
            pos = dune::utils::move(pos, d);
        }

        bool BuildingManager::Building::contains(const types::Position& p) const {
            return p.row >= pos.row && p.row < pos.row + height &&
                p.column >= pos.column && p.column < pos.column + width;
        }

        void BuildingManager::Building::take_damage(int damage) {
            health = std::max(0, health - damage);
        }

        // BuildingManager 클래스 구현
        void BuildingManager::add_building(std::unique_ptr<Building> building) {
            buildings.push_back(std::move(building));
        }

        BuildingManager::Building* BuildingManager::get_building_at(const types::Position& pos) {
            for (auto& building : buildings) {
                if (building->contains(pos)) {
                    return building.get();
                }
            }
            return nullptr;
        }

        const BuildingManager::Building* BuildingManager::get_building_at(const types::Position& pos) const {
            for (const auto& building : buildings) {
                if (building->contains(pos)) {
                    return building.get();
                }
            }
            return nullptr;
        }

        void BuildingManager::remove_building(Building* building) {
            buildings.erase(
                std::remove_if(buildings.begin(), buildings.end(),
                    [building](const std::unique_ptr<Building>& b) {
                        return b.get() == building;
                    }),
                buildings.end()
            );
        }

        const std::vector<std::unique_ptr<BuildingManager::Building>>& BuildingManager::get_buildings() const {
            return buildings;
        }

        void BuildingManager::remove_destroyed_buildings() {
            buildings.erase(
                std::remove_if(buildings.begin(), buildings.end(),
                    [](const std::unique_ptr<Building>& building) {
                        return building->is_destroyed();
                    }),
                buildings.end()
            );
        }

    } // namespace managers
} // namespace dune