#pragma once
#include "base_window.hpp"
#include <deque>
#include <string>
#include <chrono>

namespace dune {
    namespace ui {

        class MessageWindow : public BaseWindow {
        public:
            MessageWindow(int x, int y, int width, int height);
            void add_message(const std::wstring& message);
            void draw(Renderer& renderer) override;

        private:
            struct TimedMessage {
                std::wstring message;
                std::chrono::steady_clock::time_point timestamp;
                bool is_important;
            };

            static constexpr size_t MAX_MESSAGES = 8;
            static constexpr std::chrono::seconds MESSAGE_DURATION{ 5 }; // 일반 메시지 표시 시간

            std::deque<TimedMessage> messages;
            void cleanup_old_messages();

            std::wstring to_wstring(const std::string& str) const;
        };

    } // namespace ui
} // namespace dune
