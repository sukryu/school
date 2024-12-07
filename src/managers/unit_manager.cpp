#include "managers/unit_manager.hpp"
#include "utils/utils.hpp"
#include "utils/constants.hpp"
#include <iostream>

namespace dune {
    namespace managers {

        // Unit 클래스 구현

        UnitManager::Unit::Unit(types::UnitType type, int buildCost, int population, types::Position pos, int health, int speed, int attackPower, int sightRange, types::Camp camp)
                    : type_(type), buildCost_(buildCost), population_(population), position_(pos), health_(health), speed_(speed), attackPower_(attackPower), sightRange_(sightRange), camp_(camp) {}

        UnitManager::Unit::Unit(types::UnitType type, types::Position position)
            : type_(type), position_(position), lastMoveTime_(0) {
            switch (type_) {
            case types::UnitType::Sandworm:
                buildCost_ = 0;
                population_ = 0;
                speed_ = constants::SANDWORM_SPEED;
                attackPower_ = 9999;
                health_ = constants::DEFAULT_HEALTH;
                sightRange_ = 999;
                length_ = 1;
                camp_ = types::Camp::Common;
                break;
            default:
                buildCost_ = 5;
                population_ = 2;
                speed_ = 1000;
                attackPower_ = 10;
                health_ = 10;
                sightRange_ = 3;
                camp_ = types::Camp::Common;
                break;
            }
        }

        wchar_t UnitManager::Unit::getRepresentation() const {
            switch (type_) {
            case types::UnitType::Harvester:  return L'H';
            case types::UnitType::Fremen:     return L'M';
            case types::UnitType::Soldier:    return L'I';
            case types::UnitType::Fighter:    return L'J';
            case types::UnitType::HeavyTank:  return L'T';
            case types::UnitType::Sandworm:   return L'W';
            default:                          return L'?';
            }
        }

        int UnitManager::Unit::getColor() const {
            if (type_ == types::UnitType::Harvester) {
                switch (camp_) {
                case types::Camp::ArtLadies:  return constants::color::ART_LADIES;
                case types::Camp::Harkonnen:  return constants::color::HARKONNEN;
                default:                      return constants::color::OTHER;
                }
            }
            switch (type_) {
            case types::UnitType::Fremen:     return constants::color::ART_LADIES;
            case types::UnitType::Soldier:    return constants::color::ART_LADIES;
            case types::UnitType::Fighter:    return constants::color::HARKONNEN;
            case types::UnitType::HeavyTank:  return constants::color::HARKONNEN;
            case types::UnitType::Sandworm:   return constants::color::SANDWORM;
            default:                          return constants::color::OTHER;
            }
        }

        void UnitManager::Unit::printInfo() const {
            std::wcout << L"Unit Type: " << static_cast<int>(type_)
                       << L", Cost: " << buildCost_
                       << L", Population: " << population_
                       << L", Speed: " << speed_
                       << L", Attack: " << attackPower_
                       << L", Health: " << health_
                       << L", Sight: " << sightRange_
                       << L", Position: (" << position_.row << L", " << position_.column << L")\n";
        }

        void UnitManager::Unit::move(types::Direction direction) {
            position_ = utils::move(position_, direction);
        }

        bool UnitManager::Unit::isReadyToMove(std::chrono::milliseconds currentTime) const {
            return currentTime - lastMoveTime_ >= std::chrono::milliseconds(speed_);
        }

        void UnitManager::Unit::updateLastMoveTime(std::chrono::milliseconds currentTime) {
            lastMoveTime_ = currentTime;
        }

        void UnitManager::Unit::consumeTarget() {
            length_ += 1;
        }

        bool UnitManager::Unit::canExcrete() const {
            return length_ > 1;
        }

        void UnitManager::Unit::excrete() {
            if (length_ > 1) {
                length_ -= 1;
            }
        }

        bool UnitManager::Unit::shouldExcrete() const {
            return length_ > 1 && (rand() % 100 < 30);
        }

        // UnitManager 클래스 구현

        void UnitManager::addUnit(std::unique_ptr<Unit> unit) {
            types::Position pos = unit->getPosition();
            unitsByPosition_[pos] = std::move(unit);
        }

        UnitManager::Unit* UnitManager::getUnitAt(const types::Position& position) {
            auto it = unitsByPosition_.find(position);
            if (it != unitsByPosition_.end()) {
                return it->second.get();
            }
            return nullptr;
        }

        const UnitManager::Unit* UnitManager::getUnitAt(const types::Position& position) const {
            auto it = unitsByPosition_.find(position);
            if (it != unitsByPosition_.end()) {
                return it->second.get();
            }
            return nullptr;
        }

        void UnitManager::removeUnit(Unit* unit) {
            if (!unit) return;
            // Unit의 위치를 얻어 key로 사용
            types::Position pos = unit->getPosition();
            unitsByPosition_.erase(pos);
        }

        const dune::managers::UnitManager::UnitMap& 
            dune::managers::UnitManager::getUnits() const {
                return unitsByPosition_;
            }

    } // namespace managers
} // namespace dune
