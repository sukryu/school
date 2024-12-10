#include "entity/unit.hpp"
#include "utils/utils.hpp"
#include "entity/sandworm_ai.hpp"
#include "utils/constants.hpp"
#include <iostream>

namespace dune {
    namespace entity {

        // Unit 클래스 구현

        Unit::Unit(
            types::UnitType type, int buildCost, int population, types::Position pos, int health, 
            int speed, int attackPower, int sightRange, types::Camp camp
        ) : type_(type), buildCost_(buildCost), population_(population), position_(pos), health_(health), 
            speed_(speed), attackPower_(attackPower), sightRange_(sightRange), camp_(camp), harvester_ai_(nullptr) 
        {}


        Unit::Unit(types::UnitType type, types::Position position)
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

                sandworm_ai_ = std::make_unique<SandwormAI>();
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

        wchar_t Unit::getRepresentation() const {
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

        std::wstring Unit::getInfo() const {
            return L"Type: " + std::wstring(1, getRepresentation()) +
                L", Position: (" + std::to_wstring(position_.row) + L", " +
                std::to_wstring(position_.column) + L"), Health: " +
                std::to_wstring(health_);
        }


        int Unit::getColor() const {
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

        void Unit::printInfo() const {
            std::wcout << L"Unit Type: " << static_cast<int>(type_)
                << L", Cost: " << buildCost_
                << L", Population: " << population_
                << L", Speed: " << speed_
                << L", Attack: " << attackPower_
                << L", Health: " << health_
                << L", Sight: " << sightRange_
                << L", Position: (" << position_.row << L", " << position_.column << L")\n";
        }

        void Unit::move(types::Direction direction) {
            position_ = utils::move(position_, direction);
        }

        bool Unit::isReadyToMove(std::chrono::milliseconds currentTime) const {
            return currentTime - lastMoveTime_ >= std::chrono::milliseconds(speed_);
        }

        void Unit::updateLastMoveTime(std::chrono::milliseconds currentTime) {
            lastMoveTime_ = currentTime;
        }

        void Unit::consumeTarget() {
            length_ += 1;
        }

        bool Unit::canExcrete() const {
            return length_ > 1;
        }

        void Unit::excrete() {
            if (length_ > 1) {
                length_ -= 1;
            }
        }

        bool Unit::shouldExcrete() const {
            return length_ > 1 && (rand() % 100 < 30);
        }
    } // namespace entity
} // namespace dune
