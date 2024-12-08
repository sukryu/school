#pragma once
#include "../core/entity.hpp"
#include "../utils/types.hpp"
#include <memory>
#include <unordered_map>
#include <chrono>

namespace dune {
    namespace entity {
        /**
        * @brief 유닛을 나타내는 클래스입니다.
        */
        class Unit : public core::Entity {
        public:
            /**
             * @brief 일반 유닛용 생성자입니다.
             * @param type 유닛의 타입.
             * @param buildCost 생산 비용.
             * @param population 인구 수요.
             * @param speed 이동 속도.
             * @param attackPower 공격력.
             * @param health 체력.
             * @param sightRange 시야 범위.
             * @param position 유닛의 위치.
             * @param camp 유닛의 진영.
             */
            Unit(types::UnitType type, int buildCost, int population, types::Position pos, int health, int speed, int attackPower, int sightRange, types::Camp camp);

            /**
             * @brief 특수 유닛용 생성자입니다. (예: Sandworm)
             * @param type 유닛의 타입.
             * @param position 유닛의 위치.
             */
            Unit(types::UnitType type, types::Position position);

            // Entity 인터페이스 구현
            wchar_t getRepresentation() const override;
            int getColor() const override;
            void printInfo() const override;
            types::Position getPosition() const override { return position_; }
            void move(types::Direction direction) override;

            // Unit 고유 메서드

            /**
             * @brief 유닛의 타입을 반환합니다.
             * @return types::UnitType 유닛 타입.
             */
            types::UnitType getType() const { return type_; }

            /**
             * @brief 유닛의 체력을 반환합니다.
             * @return int 체력 값.
             */
            int getHealth() const { return health_; }

            /**
             * @brief 유닛의 공격력을 반환합니다.
             * @return int 공격력 값.
             */
            int getAttackPower() const { return attackPower_; }

            /**
             * @brief 유닛의 시야 범위를 반환합니다.
             * @return int 시야 범위 값.
             */
            int getSightRange() const { return sightRange_; }

            /**
             * @brief 유닛의 이동 속도를 반환합니다.
             * @return int 이동 속도 값.
             */
            int getSpeed() const { return speed_; }

            /**
             * @brief 유닛이 이동할 준비가 되었는지 확인합니다.
             * @param currentTime 현재 시간.
             * @return true 이동 가능하면 true.
             * @return false 이동 불가능하면 false.
             */
            bool isReadyToMove(std::chrono::milliseconds currentTime) const;

            /**
             * @brief 마지막 이동 시간을 업데이트합니다.
             * @param currentTime 현재 시간.
             */
            void updateLastMoveTime(std::chrono::milliseconds currentTime);

            /**
             * @brief 유닛을 지정한 위치로 이동시킵니다.
             * @param newPosition 이동할 위치.
             */
            void moveTo(const types::Position& newPosition) { position_ = newPosition; }

            // 샌드웜 전용 메서드

            /**
             * @brief 샌드웜이 타겟을 잡아먹습니다.
             */
            void consumeTarget();

            /**
             * @brief 샌드웜이 스파이스를 배출할 수 있는지 확인합니다.
             * @return true 배출 가능하면 true.
             * @return false 배출 불가능하면 false.
             */
            bool canExcrete() const;

            /**
             * @brief 샌드웜이 스파이스를 배출합니다.
             */
            void excrete();

            /**
             * @brief 샌드웜이 스파이스를 배출해야 하는지 확인합니다.
             * @return true 배출해야 하면 true.
             * @return false 그렇지 않으면 false.
             */
            bool shouldExcrete() const;

        private:
            types::Camp camp_;
            types::UnitType type_;
            int buildCost_;
            int population_;
            int speed_;
            int attackPower_;
            int health_;
            int sightRange_;
            types::Position position_;
            int length_ = 1;  // 샌드웜 길이
            std::chrono::milliseconds lastMoveTime_{ 0 };
        };
    } // namespace entity
} // namespace dune
