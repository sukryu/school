#include "entity/combat_unit_ai.hpp"
#include "entity/unit.hpp"
#include "core/map.hpp"
#include "utils/utils.hpp"

namespace dune::entity::combat {
    CombatUnitAI::CombatUnitAI(Unit* unit)
        : owner_(unit)
        , currentState_(std::make_unique<CombatIdleState>(this))
        , currentTarget_(nullptr)
        , lastAttackTime_(std::chrono::milliseconds(0))
        , moveTarget_({ -1, -1 }) {}

    void CombatUnitAI::update(core::Map& map, std::chrono::milliseconds currentTime) {
        // 상태가 있다면 업데이트 수행
        if (currentState_) {
            currentState_->update(owner_, map, currentTime);
        }

        // 시야 내의 적 탐지 (Idle 상태일 때만)
        if (currentState_->getStateName() == L"Idle") {
            detectEnemiesInSight(map);
        }
    }

    void CombatUnitAI::moveCommand(const types::Position& target) {
        moveTarget_ = target;
        CombatChangeState(std::make_unique<CombatMovingState>(this, target));
    }

    void CombatUnitAI::attackCommand(Unit* target) {
        if (!target) return;

        currentTarget_ = target;
        // 공격 범위 내에 있으면 바로 공격, 아니면 추적
        if (currentState_->isInAttackRange(owner_, target)) {
            CombatChangeState(std::make_unique<AttackingState>(this, target));
        }
        else {
            CombatChangeState(std::make_unique<PursuingState>(this, target));
        }
    }

    void CombatUnitAI::patrolCommand(const types::Position& from, const types::Position& to) {
        CombatChangeState(std::make_unique<PatrollingState>(this, from, to));
    }

    std::wstring CombatUnitAI::getCurrentState() const {
        return currentState_ ? currentState_->getStateName() : L"Unknown";
    }

    void CombatUnitAI::CombatChangeState(std::unique_ptr<CombatUnitState> newState) {
        currentState_ = std::move(newState);
    }

    void CombatUnitAI::detectEnemiesInSight(core::Map& map) {
        const auto& quadTree = map.getUnitManager().getQuadTree();
        std::vector<const entity::Unit*> nearbyUnits;

        types::Position pos = owner_->getPosition();
        int sightRange = owner_->getSightRange();

        quadTree.queryRange(
            pos.column - sightRange,
            pos.row - sightRange,
            sightRange * 2,
            sightRange * 2,
            nearbyUnits
        );

        // 가장 가까운 적 찾기
        Unit* nearestEnemy = nullptr;
        int minDistance = std::numeric_limits<int>::max();

        for (const auto* nearbyUnit : nearbyUnits) {
            // 같은 진영이면 무시
            if (nearbyUnit->getCamp() == owner_->getCamp()) continue;

            // 하베스터는 공격 우선순위가 높음
            if (nearbyUnit->getType() == types::UnitType::Harvester) {
                nearestEnemy = const_cast<Unit*>(nearbyUnit);
                break;
            }

            int distance = utils::manhattanDistance(
                owner_->getPosition(),
                nearbyUnit->getPosition()
            );

            if (distance < minDistance) {
                minDistance = distance;
                nearestEnemy = const_cast<Unit*>(nearbyUnit);
            }
        }

        // 적을 발견하면 공격
        if (nearestEnemy) {
            attackCommand(nearestEnemy);
        }
    }
} // namespace dune::entity