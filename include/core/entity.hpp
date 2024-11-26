#pragma once
#include "../utils/types.hpp"

namespace dune {
    namespace core {

        /**
         * @brief 게임 내 모든 엔티티의 기본 클래스입니다.
         */
        class Entity {
        public:
            virtual ~Entity() = default;

            /**
             * @brief 엔티티를 표현하는 문자 반환.
             * @return wchar_t 엔티티의 문자 표현.
             */
            virtual wchar_t getRepresentation() const = 0;

            /**
             * @brief 엔티티의 색상 반환.
             * @return int 콘솔에서 사용할 색상 코드.
             */
            virtual int getColor() const = 0;

            /**
             * @brief 엔티티의 정보를 출력합니다.
             */
            virtual void printInfo() const = 0;

            /**
             * @brief 엔티티의 현재 위치를 반환합니다.
             * @return types::Position 엔티티의 위치.
             */
            virtual types::Position getPosition() const = 0;

            /**
             * @brief 엔티티를 주어진 방향으로 이동합니다.
             * @param direction 이동할 방향.
             */
            virtual void move(types::Direction direction) = 0;

            /**
             * @brief 엔티티가 특정 위치에 있는지 확인합니다.
             * @param position 확인할 위치.
             * @return true 위치가 동일하면 true.
             * @return false 위치가 다르면 false.
             */
            bool isAt(const types::Position& position) const {
                return getPosition() == position;
            }

            /**
             * @brief 엔티티가 선택 가능한지 여부를 반환합니다.
             * @return true 선택 가능하면 true.
             * @return false 선택 불가능하면 false.
             */
            virtual bool isSelectable() const {
                return true;
            }

        protected:
            Entity() = default;
            Entity(const Entity&) = default;
            Entity& operator=(const Entity&) = default;
        };

    } // namespace core
} // namespace dune