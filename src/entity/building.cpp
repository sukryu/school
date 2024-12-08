#include "entity/building.hpp"
#include "managers/terrain_manager.hpp"
#include "utils/constants.hpp"
#include "utils/utils.hpp"
#include <iostream>

namespace dune {
    namespace entity {

        // Building 클래스 구현

        Building::Building(types::Camp type, const std::wstring& name, const std::wstring& description,
            int buildCost, types::Position position, int width, int height, int health, types::UnitType producedUnit)
            : type_(type)
            , name_(name)
            , description_(description)
            , buildingCost_(buildCost)
            , position_(position)
            , width_(width)
            , height_(height)
            , health_(constants::DEFAULT_HEALTH)
            , producedUnit_(producedUnit)
        {}

        wchar_t Building::getRepresentation() const {
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

        int Building::getColor() const {
            switch (type_) {
            case types::Camp::Common:     return constants::color::ART_LADIES;
            case types::Camp::ArtLadies:  return constants::color::ART_LADIES;
            case types::Camp::Harkonnen:  return constants::color::HARKONNEN;
            default:                      return constants::color::OTHER;
            }
        }

        void Building::printInfo() const {
            std::wcout << L"Building: " << name_
                << L", Description: " << description_
                << L", Cost: " << buildingCost_
                << L", Position: (" << position_.row << L", " << position_.column << L")"
                << L", Size: " << width_ << L"x" << height_
                << L", Health: " << health_ << std::endl;
        }

        void Building::move(types::Direction direction) {
            position_ = dune::utils::move(position_, direction);
        }

        bool Building::contains(const types::Position& position) const {
            return position.row >= position_.row && position.row < position_.row + height_ &&
                position.column >= position_.column && position.column < position_.column + width_;
        }

        void Building::takeDamage(int damage) {
            health_ = std::max(0, health_ - damage);
        }

        bool Building::isPlaceable(const types::Position& position, const TerrainManager& terrainManager) const {
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

    } // namespace entity
} // namespace dune
