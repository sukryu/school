#pragma once
#include "renderer.hpp"
#include "window/message_window.hpp"
#include "window/map_renderer.hpp"
#include "window/command_window.hpp"
#include "window/resource_bar.hpp"
#include "window/status_window.hpp"
#include "cursor.hpp"
#include "../utils/types.hpp"
#include "../core/map.hpp"

namespace dune {
    namespace ui {

        /**
         * @brief 게임 화면의 전체적인 렌더링을 관리하는 클래스입니다.
         */
        class Display {
        public:
            /**
             * @brief Display 클래스의 생성자입니다.
             * @param mapWidth 맵의 너비.
             * @param mapHeight 맵의 높이.
             * @param statusWidth 상태 창의 너비.
             */
            Display(int mapWidth, int mapHeight, int statusWidth = constants::DEFAULT_STATUS_WIDTH);

            /**
             * @brief 화면을 업데이트하고 렌더링합니다.
             * @param resource 자원 정보.
             * @param map 맵 객체.
             * @param cursor 커서 객체.
             */
            void update(const types::Resource& resource, const core::Map& map, const Cursor& cursor);

            /**
             * @brief 시스템 메시지를 추가합니다.
             * @param message 추가할 메시지.
             */
            void addSystemMessage(const std::wstring& message);

            /**
             * @brief 상태 메시지를 업데이트합니다.
             * @param status 업데이트할 상태 메시지.
             */
            void updateStatus(const std::wstring& status);

            /**
             * @brief 명령어 목록을 업데이트합니다.
             * @param commands 업데이트할 명령어 리스트.
             */
            void updateCommands(const std::vector<std::wstring>& commands);

            // 접근자
            MessageWindow& getMessageWindow() { return messageWindow_; }
            const MessageWindow& getMessageWindow() const { return messageWindow_; }
            int getTotalWidth() const { return totalWidth_; }
            int getTotalHeight() const { return totalHeight_; }

        private:
            Renderer renderer_;
            ResourceBar resourceBar_;
            MessageWindow messageWindow_;
            CommandWindow commandWindow_;
            StatusWindow statusWindow_;
            MapRenderer mapRenderer_;
            int totalWidth_;
            int totalHeight_;

            void renderAll();
            void clearScreen();
        };
    } // namespace ui
} // namespace dune
