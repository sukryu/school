#pragma once
#include "core/entity.hpp"
#include "utils/types.hpp"
#include "sandworm_ai.hpp"
#include "harvester_ai.hpp"
#include "combat_unit_ai.hpp"
#include <memory>
#include <chrono>
#include <string>

namespace dune {
    namespace core { class Map; }
    namespace entity { class SandwormAI; }
}

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

            std::wstring getInfo() const;

            /**
             * @brief 유닛의 진영 정보를 반환합니다.
             * @return types::Camp 유닛의 진영.
             */
            types::Camp getCamp() const { return camp_; }

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

            void initializeHarvesterAttributes();

            void initializeAI() {
                if (type_ == types::UnitType::Sandworm) {
                    sandworm_ai_ = std::make_unique<SandwormAI>();
                }
                else if (type_ == types::UnitType::Harvester) {
                    harvester_ai_ = std::make_unique<HarvesterAI>(position_);
                }
                else if (
                    type_ == types::UnitType::Soldier ||
                    type_ == types::UnitType::Fremen ||
                    type_ == types::UnitType::Fighter ||
                    type_ == types::UnitType::HeavyTank) {
                    combat_unit_ai_ = std::make_unique<combat::CombatUnitAI>(this);
                }
            }
            SandwormAI* getSandwormAI() { return sandworm_ai_.get(); }
            HarvesterAI* getHarvesterAI() { return harvester_ai_.get(); }
            combat::CombatUnitAI* getCombatUnitAI() { return combat_unit_ai_.get(); }
            void update(core::Map& map, std::chrono::milliseconds currentTime);

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
            std::unique_ptr<SandwormAI> sandworm_ai_;
            std::unique_ptr<HarvesterAI> harvester_ai_;
            std::unique_ptr<combat::CombatUnitAI> combat_unit_ai_;
        };
    } // namespace entity
} // namespace dune
