#include "ui/window/message_window.hpp"
#include "utils/constants.hpp"
#include <locale>
#include <codecvt>

/**
 * @brief 시스템 메시지를 관리하는 구현 파일입니다.
 */
namespace dune {
    namespace ui {

        MessageWindow::MessageWindow(int x, int y, int width, int height)
            : BaseWindow(x, y, width, height) {}

        std::wstring MessageWindow::toWString(const std::string& str) const {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.from_bytes(str);
        }

        void MessageWindow::addMessage(const std::wstring& message) {
            cleanupOldMessages();

            // 중요 메시지 판단 (예: 공격 받음, 유닛 파괴됨 등)
            bool isImportant = message.find(L"Attack") != std::wstring::npos ||
                message.find(L"Destroy") != std::wstring::npos ||
                message.find(L"Completed") != std::wstring::npos;

            messages_.push_back({
                message,
                std::chrono::steady_clock::now(),
                isImportant
                });

            // 최대 메시지 수 초과시 오래된 메시지 제거
            while (messages_.size() > MAX_MESSAGES) {
                messages_.pop_front();
            }
        }

        void MessageWindow::draw(Renderer& renderer) {
            cleanupOldMessages();

            drawBorder(renderer);

            int messageY = 2;
            for (const auto& timedMessage : messages_) {
                // 중요 메시지는 다른 색상으로 표시
                int color = timedMessage.isImportant ?
                    constants::color::HARKONNEN : constants::color::DEFAULT;

                drawText(renderer, 3, messageY, timedMessage.message, color);
                ++messageY;
            }
        }

        void MessageWindow::cleanupOldMessages() {
            auto now = std::chrono::steady_clock::now();

            while (!messages_.empty()) {
                auto& oldest = messages_.front();
                auto age = std::chrono::duration_cast<std::chrono::seconds>(
                    now - oldest.timestamp);

                // 중요하지 않은 메시지만 자동 삭제
                if (!oldest.isImportant && age > MESSAGE_DURATION) {
                    messages_.pop_front();
                }
                else {
                    break;
                }
            }
        }
    } // namespace ui
} // namespace dune
