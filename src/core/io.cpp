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

            // Shift 상태 확인
            bool shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;

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
            case 'p': case 'P':
                if (shiftPressed) {
                    current_key = types::Key::Build_Plate;
                }
                else {
                    current_key = types::Key::Undefined;
                }
                break;

            case 'd': case 'D':
                if (shiftPressed) {
                    current_key = types::Key::Build_Dormitory;
                }
                else {
                    current_key = types::Key::Undefined;
                }
                break;

            case 'g': case 'G':
                if (shiftPressed) {
                    current_key = types::Key::Build_Garage;
                }
                else {
                    current_key = types::Key::Undefined;
                }
                break;

            case 'k': case 'K':
                if (shiftPressed) {
                    current_key = types::Key::Build_Barracks;
                }
                else {
                    current_key = types::Key::Undefined;
                }
                break;

            case 's': case 'S':
                if (shiftPressed) {
                    current_key = types::Key::Build_Shelter;
                }
                else {
                    current_key = types::Key::Build_Soldier;
                }
                break;

            case 'a': case 'A':
                if (shiftPressed) {
                    current_key = types::Key::Build_Arena;
                }
                else {
                    current_key = types::Key::Attack;
                }
                break;

            case 'f': case 'F':
                if (shiftPressed) {
                    current_key = types::Key::Build_Factory;
                }
                else {
                    current_key = types::Key::Build_Fremen;
                }
                break;

            case 'u': case 'U':
                current_key = types::Key::ShowUnitList;
                break;
                // 유닛 명령 (Shift 없이)
            case 'h': case 'H':
                current_key = types::Key::Build_Harvester;
                break;

            case 'm': case 'M':
                current_key = types::Key::Move;
                break;

            case 'r': case 'R':
                current_key = types::Key::Build_Fighter;
                break;

            case 't': case 'T':
                current_key = types::Key::Build_HeavyTank;
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
