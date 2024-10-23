#pragma once

#include "common.hpp"

class Cursor {
public:
    Cursor(Position initial_position);
    void move(Direction dir);
    Position get_current_position() const;
    Position get_previous_position() const;

private:
    Position current;
    Position previous;
};