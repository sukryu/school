#pragma once
#include "constants.hpp"
#include <cstdint>
#include <string>

namespace dune {
	namespace types {
		/// 기본 구조체
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

            bool is_valid() const {
                return row >= 0 && row < constants::MAP_HEIGHT &&
                    column >= 0 && column < constants::MAP_WIDTH;
            }
        };

        struct Resource {
            int spice;
            int spice_max;
            int population;
            int population_max;

            Resource(int s = 0, int sm = 1000, int p = 10, int pm = 100)
                : spice(s), spice_max(sm), population(p), population_max(pm) {}
        };

        // 게임 관련 열거형
        enum class Direction {
            Stay = 0,
            Up,
            Right,
            Left,
            Down
        };

        enum class Key {
            None = 0,
            Up,
            Right,
            Left,
            Down,
            Space,
            Esc,
            Quit,
            Build,
            Train,
            Harvest,
            Move,
            Attack,
            Stop,
            Cancel,
            Build_Plate,
            Undefined
        };

        enum class SelectionType {
            None,
            Terrain,
            Building,
            Unit
        };

        enum class Camp {
            Common = 1,
            ArtLadies,
            Harkonnen
        };

        enum class UnitType {
            None,
            Harvester,
            Fremen,
            Soldier,
            Fighter,
            HeavyTank,
            Sandworm,
            DesertEagle,
            Sandstorm
        };

        enum class TerrainType {
            Desert,
            Plate,
            Rock,
            Spice,
            Empty
        };

        enum class BuildingType {
            None,
            Base,
            Plate,
            Dormitory,
            Garage,
            Barracks,
            Shelter,
            Arena,
            Factory
        };

        // 게임 상태 관련
        enum class GameState {
            Initial,
            Running,
            Paused,
            GameOver
        };
	} // namespace types
} // namespace dune
