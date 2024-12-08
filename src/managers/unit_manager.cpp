#include "managers/unit_manager.hpp"
#include "utils/utils.hpp"
#include "utils/constants.hpp"
#include <iostream>

namespace dune {
    namespace managers {
        // UnitManager 클래스 구현

        UnitManager::UnitManager()
            : quadTree_(0, 0, constants::MAP_WIDTH, constants::MAP_HEIGHT, 10) {}

        void UnitManager::addUnit(std::unique_ptr<Unit> unit) {
            types::Position pos = unit->getPosition();
            unitsByPosition_[pos] = std::move(unit);
            quadTree_.insert(unitsByPosition_[pos].get());
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
            quadTree_.remove(unit);
        }

        const UnitManager::UnitMap& 
            UnitManager::getUnits() const {
                return unitsByPosition_;
        }

    } // namespace managers
} // namespace dune
