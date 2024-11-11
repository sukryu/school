#include "ui/window/message_window.hpp"
#include <locale>
#include <codecvt>
#include "utils/constants.hpp"

namespace dune {
    namespace ui {

        MessageWindow::MessageWindow(int x, int y, int width, int height)
            : BaseWindow(x, y, width, height) {}

        std::wstring MessageWindow::to_wstring(const std::string& str) const {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.from_bytes(str);
        }

        void MessageWindow::add_message(const std::wstring& message) {
            cleanup_old_messages();

            // 중요 메시지 판단 (예: 공격 받음, 유닛 파괴됨 등)
            bool is_important = message.find(L"Attack") != std::wstring::npos ||
                message.find(L"Destoy") != std::wstring::npos ||
                message.find(L"Completed") != std::wstring::npos;

            messages.push_back({
                message,
                std::chrono::steady_clock::now(),
                is_important
                });

            // 최대 메시지 수 초과시 오래된 메시지 제거
            while (messages.size() > MAX_MESSAGES) {
                messages.pop_front();
            }
        }

        void MessageWindow::draw(Renderer& renderer) {
            cleanup_old_messages();

            draw_border(renderer);

            int message_y = 2;
            for (const auto& timed_message : messages) {
                // 중요 메시지는 다른 색상으로 표시
                int color = timed_message.is_important ?
                    constants::color::HARKONNEN : constants::color::DEFAULT;

                draw_text(renderer, 3, message_y, timed_message.message, color);
                ++message_y;
            }
        }

        void MessageWindow::cleanup_old_messages() {
            auto now = std::chrono::steady_clock::now();

            while (!messages.empty()) {
                auto& oldest = messages.front();
                auto age = std::chrono::duration_cast<std::chrono::seconds>(
                    now - oldest.timestamp);

                // 중요하지 않은 메시지만 자동 삭제
                if (!oldest.is_important && age > MESSAGE_DURATION) {
                    messages.pop_front();
                }
                else {
                    break;
                }
            }
        }
    } // namespace ui
} // namespace dune
