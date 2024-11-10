#pragma once
#include "base_window.hpp"
#include <deque>
#include <string>

namespace dune {
    namespace ui {

        class MessageWindow : public BaseWindow {
        public:
            MessageWindow(int x, int y, int width, int height);
            void add_message(const std::string& message);
            void draw(Renderer& renderer) override;

            size_t get_message_count() const { return messages.size(); }
            void clear_messages() { messages.clear(); }

        private:
            std::deque<std::string> messages;
            static constexpr size_t MAX_MESSAGES = 8;
        };
    } // namespace ui
} // namespace dune