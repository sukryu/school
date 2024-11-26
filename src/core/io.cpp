#include "core/io.hpp"
#include "utils/constants.hpp"
#include <iostream>

namespace dune {
    namespace core {
        std::chrono::steady_clock::time_point IO::lastKeyTime_;
        types::Key IO::lastKey_ = types::Key::None;
        bool IO::wasDoubleClick_ = false;

        types::Key IO::getKey() {
            if (!_kbhit()) {
                wasDoubleClick_ = false;
                return types::Key::None;
            }

            wint_t byte = _getwch();
            types::Key current_key;

            switch (byte) {
            case L'q': case L'Q':
                current_key = types::Key::Quit;
                break;
            case L' ': 
                current_key = types::Key::Space;
                break;
            case 27:  // ESC
                current_key = types::Key::Esc;
                break;
            case 'p':
            case 'P':
                current_key = types::Key::Build_Plate;
                break;
            case 'h':
            case 'H':
                current_key = types::Key::Build_Harvester;
                break;
            case 224: case 0: {  // Special keys (arrows)
                byte = _getwch();
                switch (byte) {
                case 72:  // Up arrow
                    current_key = types::Key::Up;
                    break;
                case 75:  // Left arrow
                    current_key = types::Key::Left;
                    break;
                case 77:  // Right arrow
                    current_key = types::Key::Right;
                    break;
                case 80:  // Down arrow
                    current_key = types::Key::Down;
                    break;
                default:
                    current_key = types::Key::Undefined;
                    break;
                }
                break;
            }
            default:
                current_key = types::Key::Undefined;
                break;
            }

            auto now = std::chrono::steady_clock::now();
            checkDoubleClick(current_key, now);

            if (current_key != types::Key::None) {
                lastKey_ = current_key;
                lastKeyTime_ = now;
            }

            return current_key;
        }

        bool IO::isDoubleClick() {
            return wasDoubleClick_;
        }

        bool IO::checkDoubleClick(types::Key current_key, std::chrono::steady_clock::time_point now) {
            if (current_key != lastKey_) {
                wasDoubleClick_ = false;
                return false;
            }

            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastKeyTime_);
            bool is_double = diff < DOUBLE_CLICK_INTERVAL_;

            if (is_double) {
                wasDoubleClick_ = true;
                lastKeyTime_ = now - std::chrono::milliseconds(500);
            }

            return wasDoubleClick_;
        }
    } // namespace core
} // namespace dune
