#include "unit_manager.hpp"

void UnitManager::add_unit(std::unique_ptr<Unit> unit) {
    units.push_back(std::move(unit));
}

const std::vector<std::unique_ptr<Unit>>& UnitManager::get_units() const {
    return units;
}

const Unit* UnitManager::get_unit_at(const Position& pos) const {
    for (const auto& unit : units) {
        if (unit->get_position() == pos) {
            return unit.get();
        }
    }
    return nullptr;
}