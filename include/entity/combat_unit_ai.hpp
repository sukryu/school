#pragma once
#include "combat_unit_state.hpp"
#include <memory>
#include <chrono>
#include <string>

namespace dune::entity::combat {
    /**
     * @brief 전투 유닛의 AI를 관리하는 클래스입니다.
     */
    class CombatUnitAI {
    public:
        /**
         * @brief 생성자
         * @param unit AI가 제어할 유닛
         */
        explicit CombatUnitAI(Unit* unit);
        ~CombatUnitAI() = default;

        /**
         * @brief AI 상태를 업데이트합니다.
         */
        void update(core::Map& map, std::chrono::milliseconds currentTime);

        /**
         * @brief 이동 명령을 내립니다.
         */
        void moveCommand(const types::Position& target);

        /**
         * @brief 공격 명령을 내립니다.
         */
        void attackCommand(Unit* target);

        /**
         * @brief 순찰 명령을 내립니다.
         */
        void patrolCommand(const types::Position& from, const types::Position& to);

        /**
         * @brief 현재 상태의 이름을 반환합니다.
         */
        std::wstring getCurrentState() const;

        /**
         * @brief 상태를 변경합니다.
         */
        void CombatChangeState(std::unique_ptr<CombatUnitState> newState);

        /**
         * @brief 현재 타겟을 반환합니다.
         */
        Unit* getCurrentTarget() const { return currentTarget_; }

        /**
         * @brief 현재 타겟을 설정합니다.
         */
        void setCurrentTarget(Unit* target) { currentTarget_ = target; }

        /**
         * @brief 마지막 공격 시간을 업데이트합니다.
         */
        void updateLastAttackTime(std::chrono::milliseconds time) {
            lastAttackTime_ = time;
        }

        void detectEnemiesInSight(core::Map& map);

    private:
        Unit* owner_;                                       // AI가 제어하는 유닛
        std::unique_ptr<CombatUnitState> currentState_;    // 현재 상태
        Unit* currentTarget_;                              // 현재 타겟
        std::chrono::milliseconds lastAttackTime_;         // 마지막 공격 시간
        types::Position moveTarget_;                       // 이동 목표 위치
    };
}