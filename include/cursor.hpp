#pragma once

#include "common.hpp"
#include "renderer.hpp"
#include "map.hpp"

class Cursor {
public:
    Cursor(Position initial_position);
    void move(Direction dir);
    Position get_current_position() const;
    Position get_previous_position() const;
    void draw(Renderer& renderer, const Map& map) const;

private:
    Position current;
    Position previous;
};