#include "utils.hpp"
#include <cmath>

namespace dune {
    namespace utils {

        types::Position move(const types::Position& position, types::Direction direction) {
            types::Position newPosition = position;
            switch (direction) {
            case types::Direction::Up:
                newPosition.row--;
                break;
            case types::Direction::Down:
                newPosition.row++;
                break;
            case types::Direction::Left:
                newPosition.column--;
                break;
            case types::Direction::Right:
                newPosition.column++;
                break;
            }
            return newPosition;
        }

        int manhattanDistance(const types::Position& a, const types::Position& b) {
            return std::abs(a.row - b.row) + std::abs(a.column - b.column);
        }

    } // namespace utils
} // namespace dune
