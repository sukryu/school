#pragma once
#include "../utils/types.hpp"
#include <chrono>
#include <vector>
#include <string>

namespace dune {
    namespace core { class Map; }
    namespace entity {
        class Unit;
        namespace combat {
            class CombatUnitAI;
        }
    }
}

namespace dune::entity::combat {
    /**
     * @brief 전투 유닛의 기본 상태 클래스입니다.
     */
    class CombatUnitState {
    public:
        virtual ~CombatUnitState() = default;

        /**
         * @brief 상태를 업데이트합니다.
         */
        virtual void update(Unit* unit, core::Map& map,
            std::chrono::milliseconds currentTime) = 0;

        /**
         * @brief 현재 상태의 이름을 반환합니다.
         */
        virtual std::wstring getStateName() const = 0;

        /**
         * @brief 공격 가능한 범위인지 확인합니다.
         */
        bool isInAttackRange(const Unit* attacker, const Unit* target) const;

        /**
         * @brief 시야 범위 내에 있는지 확인합니다.
         */
        bool isInSightRange(const Unit* unit, const types::Position& pos) const;

    protected:
        /**
         * @brief 주어진 위치로의 경로를 찾습니다.
         */
        std::vector<types::Position> findPath(
            const types::Position& start,
            const types::Position& goal,
            const core::Map& map) const;

        /**
         * @brief 이동 가능한 위치인지 확인합니다.
         */
        bool isValidMovePosition(const types::Position& pos, const core::Map& map) const;

        /**
         * @brief 공격이 가능한 상태인지 확인합니다.
         */
        bool canAttack(const Unit* unit, std::chrono::milliseconds currentTime) const;
    };

    /**
     * @brief 대기 상태 클래스입니다.
     */
    class CombatIdleState : public CombatUnitState {
    public:
        explicit CombatIdleState(CombatUnitAI* ai);
        void update(Unit* unit, core::Map& map,
            std::chrono::milliseconds currentTime) override;
        std::wstring getStateName() const override { return L"Idle"; }
    private:
        CombatUnitAI* ai_;
    };

    /**
     * @brief 이동 상태 클래스입니다.
     */
    class CombatMovingState : public CombatUnitState {
    public:
        CombatMovingState(CombatUnitAI* ai, const types::Position& target);
        void update(Unit* unit, core::Map& map,
            std::chrono::milliseconds currentTime) override;
        std::wstring getStateName() const override { return L"Moving"; }
    private:
        CombatUnitAI* ai_;
        types::Position targetPosition_;
        std::vector<types::Position> currentPath_;
    };

    /**
     * @brief 공격 상태 클래스입니다.
     */
    class AttackingState : public CombatUnitState {
    public:
        AttackingState(CombatUnitAI* ai, Unit* target);
        void update(Unit* unit, core::Map& map,
            std::chrono::milliseconds currentTime) override;
        std::wstring getStateName() const override {
            return L"Attacking";
        }
        std::wstring getUnitTypeName(types::UnitType type) const;
    private:
        CombatUnitAI* ai_;
        Unit* target_;
        std::chrono::milliseconds lastAttackTime_;
    };

    /**
     * @brief 순찰 상태 클래스입니다.
     */
    class PatrollingState : public CombatUnitState {
    public:
        PatrollingState(CombatUnitAI* ai,
            const types::Position& from,
            const types::Position& to);
        void update(Unit* unit, core::Map& map,
            std::chrono::milliseconds currentTime) override;
        std::wstring getStateName() const override { return L"Patrolling"; }
    private:
        CombatUnitAI* ai_;
        types::Position fromPosition_;
        types::Position toPosition_;
        types::Position currentTarget_;
        std::vector<types::Position> currentPath_;
    };

    /**
     * @brief 추적 상태 클래스입니다.
     */
    class PursuingState : public CombatUnitState {
    public:
        PursuingState(CombatUnitAI* ai, Unit* target);
        void update(Unit* unit, core::Map& map,
            std::chrono::milliseconds currentTime) override;
        std::wstring getStateName() const override { return L"Pursuing"; }
    private:
        CombatUnitAI* ai_;
        Unit* target_;
        std::vector<types::Position> currentPath_;
        std::chrono::milliseconds lastPathUpdateTime_;
    };
}