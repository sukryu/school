#pragma once

#include "common.hpp"
#include "map.hpp"
#include "cursor.hpp"
#include "display.hpp"
#include "io.hpp"
#include <chrono>
#include <memory>
#include <vector>

class Game {
public:
    Game();
    void run();

private:
    void init();
    void init_resources();
    void init_buildings();
    void init_terrain();
    void init_sandworms();
    void init_harvesters();
    void init_map();
    void init_display();
    void intro();
    void outro();

    Selection current_selection;
    void handle_movement(Key key);
    void handle_selection();
    void handle_escape();
    void update_selection_display();

    std::chrono::milliseconds sys_clock;
    Map map;
    Resource resource;
    Cursor cursor;
    Display display;
};