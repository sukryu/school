#pragma once

#include <Windows.h>
#include <cstdint>
#include <array>
#include <chrono>
#include <concepts>
#include <iostream>
#include <vector>

constexpr int TICK = 10; // time unit(ms)
constexpr int N_LAYER = 2;
constexpr int MAP_WIDTH = 60;
constexpr int MAP_HEIGHT = 18;
constexpr int RESOURCE_HEIGHT = 1;
constexpr int SYSTEM_MESSAGE_HEIGHT = 8;

static constexpr int COLOR_DEFAULT = 15;
static constexpr int COLOR_CURSOR = 112;
static constexpr int COLOR_RESOURCE = 112;

static constexpr int COLOR_ART_LADIES = 9;   // Blue
static constexpr int COLOR_HARKONNEN = 12;   // Red
static constexpr int COLOR_SANDWORM = 14;    // Yellow
static constexpr int COLOR_PLATE = 0;        // Black
static constexpr int COLOR_SPICE = 6;        // Orange
static constexpr int COLOR_OTHER = 8;        // Gray

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

struct Selection {
    SelectionType type = SelectionType::None;
    Position position = { 0, 0 };
    const void* selected_ptr = nullptr;

    void clear() {
        type = SelectionType::None;
        position = { 0, 0 };
        selected_ptr = nullptr;
    }
};

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
    sandworm
};

enum class TerrainType {
    Desert, // 기본 사막
    Plate, // 장판 (건설 가능)
    Rock, // 바위 (이동 불가)
    Spice, // 스파이스 매장지
    Empty // 빈 공간 (테두리 등)
};

class Unit {
public:
    Unit(UnitType type, int build_cost, int population, int speed, int attack_power, int health, int sight_range, Position pos)
        : type(type), build_cost(build_cost), population(population), speed(speed),
        attack_power(attack_power), health(health), sight_range(sight_range), pos(pos) {}

    char get_representation() const {
        switch (type) {
        case UnitType::harvester: return 'H';
        case UnitType::fremen: return 'M';
        case UnitType::soldier: return 'I';
        case UnitType::fighter: return 'J';
        case UnitType::heavy_tank: return 'T';
        case UnitType::sandworm: return 'Z';
        default: return '?';
        }
    }

    // 유닛 타입에 따른 색상 반환
    int get_color() const {
        switch (type) {
        case UnitType::harvester:  return COLOR_OTHER;
        case UnitType::fremen:     return COLOR_ART_LADIES;
        case UnitType::soldier:    return COLOR_ART_LADIES;
        case UnitType::fighter:    return COLOR_HARKONNEN;
        case UnitType::heavy_tank: return COLOR_HARKONNEN;
        case UnitType::sandworm:   return COLOR_SANDWORM;
        default: return COLOR_OTHER;
        }
    }

    void print_info() const {
        std::cout << "Unit Type: " << static_cast<int>(type)
            << ", Cost: " << build_cost
            << ", Population: " << population
            << ", Speed: " << speed
            << ", Attack: " << attack_power
            << ", Health: " << health
            << ", Sight: " << sight_range
            << ", Position: (" << pos.row << ", " << pos.column << ")\n";
    }

    Position get_position() const {
        return pos;
    }

    UnitType get_type() const {
        return type;
    }

    void move(Direction d) {
        pos = ::move(pos, d);
    }

private:
    UnitType type;
    int build_cost;
    int population;
    int speed;
    int attack_power;
    int health;
    int sight_range;
    Position pos;
};

class Building {
public:
    Building(Camp type, const char* name, const char* description, int build_cost, Position pos, int width, int height, UnitType produced_unit)
        : type(type), name(name), description(description), building_cost(build_cost),
        pos(pos), width(width), height(height), produced_unit(produced_unit) {}

    char get_representation() const {
        if (strcmp(name, "Base") == 0) return 'B';
        if (strcmp(name, "Plate") == 0) return 'P';
        if (strcmp(name, "Dormitory") == 0) return 'D';
        if (strcmp(name, "Garage") == 0) return 'G';
        if (strcmp(name, "Barracks") == 0) return 'K';  // Barracks 문자
        if (strcmp(name, "Shelter") == 0) return 'S';
        if (strcmp(name, "Arena") == 0) return 'A';
        if (strcmp(name, "Factory") == 0) return 'F';
        return '?';
    }

    // 건물 타입에 따른 색상 반환
    int get_color() const {
        switch (type) {
        case Camp::common:      return COLOR_OTHER;
        case Camp::art_ladies:  return COLOR_ART_LADIES;
        case Camp::harkonnen:   return COLOR_HARKONNEN;
        default: return COLOR_OTHER;
        }
    }

    void print_info() const {
        std::cout << "Building: " << name
            << ", Description: " << description
            << ", Cost: " << building_cost
            << ", Position: (" << pos.row << ", " << pos.column << ")"
            << ", Size: " << width << "x" << height
            << ", Produced Unit: " << static_cast<int>(produced_unit) << std::endl;
    }

    // 특정 좌표가 건물 내부에 있는지 확인
    bool contains(const Position& p) const {
        return p.row >= pos.row && p.row < pos.row + height &&
            p.column >= pos.column && p.column < pos.column + width;
    }

    Camp get_type() const {
        return type;
    }

    const char* get_name() const {
        return name;
    }

    const int get_height() const {
        return height;
    }

    const int get_width() const {
        return width;
    }

    Position get_position() const {
        return pos;
    }

    // 건물 위치를 이동
    void move(Direction d) {
        pos = ::move(pos, d);
    }

private:
    Camp type;
    const char* name;
    const char* description;
    int building_cost;
    Position pos;
    int width;
    int height;
    UnitType produced_unit;
};

class Terrain {
public:
    Terrain(TerrainType type = TerrainType::Desert) : type(type) {}

    char get_representation() const {
        switch (type) {
        case TerrainType::Desert: return ' ';    // 사막은 빈 칸으로 표시
        case TerrainType::Plate:  return 'P';    // 장판
        case TerrainType::Rock:   return 'R';    // 바위
        case TerrainType::Spice:  return '5';    // 스파이스 매장지
        case TerrainType::Empty:  return '#';    // 테두리/빈 공간
        default: return '?';
        }
    }

    int get_color() const {
        switch (type) {
        case TerrainType::Desert: return COLOR_DEFAULT;   // 기본 색상
        case TerrainType::Plate:  return COLOR_PLATE;     // 검정
        case TerrainType::Rock:   return COLOR_OTHER;     // 회색
        case TerrainType::Spice:  return COLOR_SPICE;    // 주황
        case TerrainType::Empty:  return COLOR_DEFAULT;   // 기본 색상
        default: return COLOR_DEFAULT;
        }
    }

    bool is_buildable() const {
        switch (type) {
        case TerrainType::Desert: return true;
        case TerrainType::Plate:  return true;
        case TerrainType::Rock:   return false;
        case TerrainType::Spice:  return false;
        case TerrainType::Empty:  return false;
        default: return false;
        }
    }

    bool is_walkable() const {
        switch (type) {
        case TerrainType::Desert: return true;
        case TerrainType::Plate:  return true;
        case TerrainType::Rock:   return false;
        case TerrainType::Spice:  return true;
        case TerrainType::Empty:  return false;
        default: return false;
        }
    }

    // 스파이스 수확 가능 여부
    bool can_harvest_spice() const {
        return type == TerrainType::Spice;
    }

    TerrainType get_type() const {
        return type;
    }

private:
    TerrainType type;
};

struct Tile {
    Terrain terrain;
    char object;  // 건물이나 유닛 등의 오브젝트

    Tile() : terrain(TerrainType::Desert), object(' ') {}
    Tile(TerrainType type) : terrain(type), object(' ') {}
};