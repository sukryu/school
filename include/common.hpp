#pragma once

#include <Windows.h>
#include <cstdint>
#include <array>
#include <chrono>
#include <concepts>
#include <iostream>
#include <vector>
#include <algorithm>

// 상수 정의
constexpr int TICK = 10; // time unit(ms)
constexpr int N_LAYER = 2;
constexpr int MAP_WIDTH = 60;
constexpr int MAP_HEIGHT = 18;
constexpr int RESOURCE_HEIGHT = 1;
constexpr int SYSTEM_MESSAGE_HEIGHT = 8;

// 색상 상수
static constexpr int COLOR_DEFAULT = 15;
static constexpr int COLOR_CURSOR = 112;
static constexpr int COLOR_RESOURCE = 112;

static constexpr int COLOR_ART_LADIES = 9;   // Blue
static constexpr int COLOR_HARKONNEN = 12;   // Red
static constexpr int COLOR_SANDWORM = 14;    // Yellow
static constexpr int COLOR_PLATE = 0;        // Black
static constexpr int COLOR_SPICE = 6;        // Orange
static constexpr int COLOR_OTHER = 8;        // Gray

// 기본 구조체
struct Resource {
    int spice;
    int spice_max;
    int population;
    int population_max;
};

struct Position {
    int row, column;

    constexpr Position operator+(const Position& other) const {
        return { row + other.row, column + other.column };
    }

    constexpr Position operator-(const Position& other) const {
        return { row - other.row, column - other.column };
    }

    bool operator==(const Position& other) const {
        return row == other.row && column == other.column;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

// 열거형 정의
enum class Direction {
    Stay = 0, Up, Right, Left, Down
};

enum class Key {
    None = 0, Up, Right, Left, Down, Space, Esc, Quit, Undefined
};

enum class SelectionType {
    None,
    Terrain,
    Building,
    Unit
};

enum class Camp {
    common = 1,
    art_ladies,
    harkonnen
};

enum class UnitType {
    harvester,
    fremen,
    soldier,
    fighter,
    heavy_tank,
    sandworm,
    desert_eagle,
    sandstorm
};

enum class TerrainType {
    Desert,
    Plate,
    Rock,
    Spice,
    Empty
};

// 선택 상태를 저장하는 구조체
struct Selection {
    SelectionType type = SelectionType::None;
    Position position = { 0, 0 };
    void* selected_ptr = nullptr;

    void clear() {
        type = SelectionType::None;
        position = { 0, 0 };
        selected_ptr = nullptr;
    }
};

// 유틸리티 함수
constexpr bool is_arrow_key(Key k) {
    return k >= Key::Up && k <= Key::Down;
}

constexpr Direction key_to_direction(Key k) {
    return static_cast<Direction>(k);
}

constexpr Position direction_to_position(Direction d) {
    constexpr std::array<Position, 5> direction_vector = {
        Position{0, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 0}
    };
    return direction_vector[static_cast<int>(d)];
}

constexpr Position move(const Position& p, Direction d) {
    return p + direction_to_position(d);
}