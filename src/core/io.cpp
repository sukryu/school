#include "core/io.hpp"
#include "utils/constants.hpp"
#include <iostream>

namespace dune {
    namespace core {
        std::chrono::steady_clock::time_point IO::last_key_time;
        types::Key IO::last_key = types::Key::None;
        bool IO::was_double_click = false;

        types::Key IO::get_key() {
            if (!_kbhit()) {
                was_double_click = false;
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
            check_double_click(current_key, now);

            if (current_key != types::Key::None) {
                last_key = current_key;
                last_key_time = now;
            }

            return current_key;
        }

        bool IO::is_double_click() {
            return was_double_click;
        }

        bool IO::check_double_click(types::Key current_key, std::chrono::steady_clock::time_point now) {
            if (current_key != last_key) {
                was_double_click = false;
                return false;
            }

            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_key_time);
            bool is_double = diff < DOUBLE_CLICK_INTERVAL;

            if (is_double) {
                was_double_click = true;
                last_key_time = now - std::chrono::milliseconds(500);
            }

            return was_double_click;
        }
    } // namespace core
} // namespace dune
