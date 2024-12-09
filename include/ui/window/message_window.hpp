#pragma once
#include "base_window.hpp"
#include <deque>
#include <string>
#include <chrono>

namespace dune {
    namespace ui {

        /**
         * @brief 시스템 메시지를 표시하는 윈도우 클래스입니다.
         */
        class MessageWindow : public BaseWindow {
        public:
            /**
             * @brief MessageWindow 클래스의 생성자입니다.
             * @param x 윈도우의 x 좌표.
             * @param y 윈도우의 y 좌표.
             * @param width 윈도우의 너비.
             * @param height 윈도우의 높이.
             */
            MessageWindow(int x, int y, int width, int height);

            /**
             * @brief 메시지를 추가합니다.
             * @param message 추가할 메시지.
             */
            void addMessage(const std::wstring& message);

            /**
             * @brief 윈도우를 그립니다.
             * @param renderer 렌더러 객체.
             */
            void draw(Renderer& renderer) override;

            /**
         * @brief 시간 정보가 포함된 메시지 구조체입니다.
         */
            struct TimedMessage {
                std::wstring message;
                std::chrono::steady_clock::time_point timestamp;
                bool isImportant;
                bool isProcessed;  // 메시지 처리 여부

                TimedMessage(const std::wstring& msg, bool important = false)
                    : message(msg)
                    , timestamp(std::chrono::steady_clock::now())
                    , isImportant(important)
                    , isProcessed(false) {}
            };

            const std::deque<TimedMessage>& getMessages() const {
                return messages_;
            }

            /**
             * @brief 특정 메시지를 처리 완료로 표시합니다.
             */
            void markMessageProcessed(const std::wstring& message) {
                for (auto& timedMessage : messages_) {
                    if (timedMessage.message == message && !timedMessage.isProcessed) {
                        timedMessage.isProcessed = true;
                        break;
                    }
                }
            }

        private:

            static constexpr size_t MAX_MESSAGES = 8;
            static constexpr std::chrono::seconds MESSAGE_DURATION{ 5 }; // 일반 메시지 표시 시간

            std::deque<TimedMessage> messages_;
            void cleanupOldMessages();

            std::wstring toWString(const std::string& str) const;
        };

    } // namespace ui
} // namespace dune
