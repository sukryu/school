#pragma once

#include "common.hpp"
#include <vector>
#include <memory>

class UnitManager {
public:
    void add_unit(std::unique_ptr<Unit> unit);
    const std::vector<std::unique_ptr<Unit>>& get_units() const;
    const Unit* get_unit_at(const Position& pos) const;

private:
    std::vector<std::unique_ptr<Unit>> units;
};