#pragma once
#include "constants.hpp"
#include <cstdint>
#include <string>

namespace dune {
	namespace types {
        /**
         * @brief 맵 상의 위치를 나타내는 구조체입니다.
         */
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

        /**
         * @brief 자원 정보를 담는 구조체입니다.
         */
        struct Resource {
            int spice;
            int spice_max;
            int population;
            int population_max;

            Resource(int s = 0, int sm = 1000, int p = 10, int pm = 100)
                : spice(s), spice_max(sm), population(p), population_max(pm) {}
        };

        /**
         * @brief 방향을 나타내는 열거형입니다.
         */
        enum class Direction {
            Stay = 0,
            Up,
            Right,
            Left,
            Down
        };

        /**
         * @brief 사용자 입력 키를 나타내는 열거형입니다.
         */
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
            Build_Harvester,
            Undefined
        };

        /**
         * @brief 선택된 객체의 타입을 나타내는 열거형입니다.
         */
        enum class SelectionType {
            None,
            Terrain,
            Building,
            Unit
        };

        /**
         * @brief 진영을 나타내는 열거형입니다.
         */
        enum class Camp {
            Common = 1,
            ArtLadies,
            Harkonnen
        };

        /**
         * @brief 유닛의 종류를 나타내는 열거형입니다.
         */
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

        /**
         * @brief 지형의 종류를 나타내는 열거형입니다.
         */
        enum class TerrainType {
            Desert,
            Plate,
            Rock,
            Spice,
            Empty
        };

        /**
         * @brief 건물의 종류를 나타내는 열거형입니다.
         */
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
