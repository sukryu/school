#pragma once
#include "types.hpp"
#include "constants.hpp"
#include <chrono>
#include <random>

namespace dune {
	namespace utils {
        // 키 관련 유틸리티
        constexpr bool is_arrow_key(types::Key k) {
            return k >= types::Key::Up && k <= types::Key::Down;
        }

        constexpr types::Direction key_to_direction(types::Key k) {
            switch (k) {
            case types::Key::Up: return types::Direction::Up;
            case types::Key::Right: return types::Direction::Right;
            case types::Key::Left: return types::Direction::Left;
            case types::Key::Down: return types::Direction::Down;
            default: return types::Direction::Stay;
            }
        }

        // 위치 관련 유틸리티
        constexpr types::Position direction_to_position(types::Direction d) {
            switch (d) {
            case types::Direction::Up: return { -1, 0 };
            case types::Direction::Right: return { 0, 1 };
            case types::Direction::Left: return { 0, -1 };
            case types::Direction::Down: return { 1, 0 };
            default: return { 0, 0 };
            }
        }

        /**
         * @brief 주어진 방향으로 위치를 이동시킵니다.
         * @param position 현재 위치.
         * @param direction 이동할 방향.
         * @return types::Position 이동한 후의 위치.
         */
        types::Position move(const types::Position& position, types::Direction direction);

        /**
         * @brief 두 위치 간의 맨해튼 거리를 계산합니다.
         * @param a 위치 A.
         * @param b 위치 B.
         * @return int 맨해튼 거리.
         */
        int manhattanDistance(const types::Position& a, const types::Position& b);

        // 난수 생성
        inline int random_int(int min, int max) {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(min, max);
            return dis(gen);
        }

        // 시간 관련 유틸리티
        using GameClock = std::chrono::steady_clock;
        using TimePoint = GameClock::time_point;
        using Duration = std::chrono::milliseconds;

        inline TimePoint now() {
            return GameClock::now();
        }

        inline Duration elapsed_time(const TimePoint& start) {
            return std::chrono::duration_cast<Duration>(now() - start);
        }
	} // namespace utils
} // namespace dune