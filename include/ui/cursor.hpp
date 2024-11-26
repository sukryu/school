#pragma once
#include "../utils/types.hpp"
#include "../core/map.hpp"
#include "../managers/unit_manager.hpp"
#include "../managers/building_manager.hpp"
#include "renderer.hpp"

namespace dune {
    namespace ui {

        /**
         * @brief 맵 상에서 사용자의 선택을 나타내는 커서를 관리하는 클래스입니다.
         */
        class Cursor {
        public:
            using Unit = managers::Unit;
            using Building = managers::BuildingManager::Building;

            /**
             * @brief Cursor 클래스의 생성자입니다.
             * @param initialPosition 초기 커서 위치.
             */
            explicit Cursor(types::Position initialPosition);

            /**
             * @brief 커서를 주어진 방향으로 이동합니다.
             * @param direction 이동할 방향.
             */
            void move(types::Direction direction);

            /**
             * @brief 현재 커서 위치를 반환합니다.
             * @return types::Position 현재 커서 위치.
             */
            types::Position getCurrentPosition() const { return current_; }

            /**
             * @brief 이전 커서 위치를 반환합니다.
             * @return types::Position 이전 커서 위치.
             */
            types::Position getPreviousPosition() const { return previous_; }

            /**
             * @brief 커서를 화면에 그립니다.
             * @param renderer 렌더러 객체.
             * @param map 맵 객체.
             */
            void draw(Renderer& renderer, const core::Map& map) const;

            /**
             * @brief 이동 가능한 위치인지 확인합니다.
             * @param direction 이동할 방향.
             * @param map 맵 객체.
             * @return true 이동 가능하면 true.
             * @return false 이동 불가능하면 false.
             */
            bool isValidMove(types::Direction direction, const core::Map& map) const;

            /**
             * @brief 커서의 위치를 설정합니다.
             * @param position 설정할 위치.
             */
            void setPosition(types::Position position) { previous_ = current_; current_ = position; }

        private:
            types::Position current_;
            types::Position previous_;
            void restorePreviousPosition(Renderer& renderer, const core::Map& map) const;
            void drawCursor(Renderer& renderer) const;
        };

    } // namespace ui
} // namespace dune
