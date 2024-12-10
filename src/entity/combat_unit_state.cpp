#include "entity/combat_unit_state.hpp"
#include "core/map.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <queue>
#include <unordered_set>
#include <array>

namespace dune::entity::combat {

    // A* 알고리즘을 이용한 경로 찾기 (하베스터와 동일)
    std::vector<types::Position> CombatUnitState::findPath(
        const types::Position& start,
        const types::Position& goal,
        const core::Map& map
    ) const {
        // 방향 설정 (상, 하, 좌, 우)
        const std::array<types::Position, 4> directions = {
            types::Position{-1, 0}, // 위
            types::Position{1, 0}, // 아래
            types::Position{0, -1}, // 왼쪽
            types::Position{0, 1}  // 오른쪽
        };

        // 우선순위 큐 및 노드 관리
        auto cmp = [](const types::Node& a, const types::Node& b) {
            return a.fCost() > b.fCost();
            };
        std::priority_queue<types::Node, std::vector<types::Node>, decltype(cmp)> openList(cmp);
        std::unordered_map<types::Position, types::Node> closedList;

        // 시작 노드 초기화
        openList.push({ start, 0, utils::manhattanDistance(start, goal), { -1, -1 } });

        while (!openList.empty()) {
            // 최상위 노드 가져오기
            types::Node currentNode = openList.top();
            openList.pop();

            // 목표 지점에 도달하면 경로 재구성
            if (currentNode.position == goal) {
                std::vector<types::Position> path;
                types::Position currentPos = goal;
                while (currentPos != start) {
                    path.push_back(currentPos);
                    currentPos = closedList[currentPos].parent; // 부모 노드로 이동
                }
                std::reverse(path.begin(), path.end());
                return path; // 완성된 경로 반환
            }

            // 현재 노드를 닫힌 리스트에 추가
            closedList[currentNode.position] = currentNode;

            // 이웃 노드 탐색
            for (const auto& dir : directions) {
                types::Position neighborPos = currentNode.position + dir;

                // 맵 경계 확인
                if (!neighborPos.is_valid() ||
                    neighborPos.row >= map.getHeight() ||
                    neighborPos.column >= map.getWidth()) {
                    continue;
                }

                // 장애물 확인
                const auto& terrain = map.getTerrainManager().getTerrain(neighborPos);
                if (terrain.getType() == types::TerrainType::Rock ||
                    map.getEntityAt<dune::entity::Building>(neighborPos) ||
                    map.getEntityAt<dune::entity::Unit>(neighborPos)) {
                    continue; // 장애물이 있는 경우 무시
                }

                // 비용 계산
                int newGCost = currentNode.gCost + 1; // 기본 이동 비용
                int hCost = utils::manhattanDistance(neighborPos, goal);

                // 이미 닫힌 리스트에 있는 노드는 무시
                if (closedList.find(neighborPos) != closedList.end() &&
                    closedList[neighborPos].gCost <= newGCost) {
                    continue;
                }

                // 우선순위 큐에 추가
                openList.push({ neighborPos, newGCost, hCost, currentNode.position });
            }
        }

        // 경로를 찾지 못한 경우
        return {};
    }

    bool CombatUnitState::isInAttackRange(
        const Unit* attacker,
        const Unit* target
    ) const {
        if (!attacker || !target) return false;

        int distance = utils::manhattanDistance(
            attacker->getPosition(),
            target->getPosition()
        );

        // 각 유닛의 특성에 따른 공격 범위 설정
        int attackRange;
        switch (attacker->getType()) {
        case types::UnitType::Soldier:    // 보병
        case types::UnitType::Fighter:    // 투사
            attackRange = 1;  // 근접 공격
            break;
        case types::UnitType::Fremen:     // 프레멘
            attackRange = 2;  // 중거리 공격
            break;
        case types::UnitType::HeavyTank:  // 중전차
            attackRange = 3;  // 장거리 공격
            break;
        default:
            return false;  // 공격 불가능한 유닛
        }

        return distance <= attackRange;
    }

    bool CombatUnitState::isInSightRange(
        const Unit* unit,
        const types::Position& pos
    ) const {
        if (!unit) return false;

        int distance = utils::manhattanDistance(
            unit->getPosition(),
            pos
        );
        return distance <= unit->getSightRange();
    }

    bool CombatUnitState::canAttack(
        const Unit* unit,
        std::chrono::milliseconds currentTime
    ) const {
        // 공격 쿨다운 체크
        return unit->isReadyToMove(currentTime); // 이동과 동일한 쿨다운 사용
    }

    // CombatIdleState 구현
    CombatIdleState::CombatIdleState(CombatUnitAI* ai)
        : ai_(ai) {}

    void CombatIdleState::update(
        Unit* unit,
        core::Map& map,
        std::chrono::milliseconds currentTime
    ) {
        // Idle 상태에서는 특별한 동작 없음
        // 적 탐지는 CombatUnitAI::update에서 처리
    }

    // CombatMovingState 구현
    CombatMovingState::CombatMovingState(CombatUnitAI* ai, const types::Position& target)
        : ai_(ai)
        , targetPosition_(target)
        , currentPath_() {}

    void CombatMovingState::update(
        Unit* unit,
        core::Map& map,
        std::chrono::milliseconds currentTime
    ) {
        if (currentPath_.empty()) {
            currentPath_ = findPath(unit->getPosition(), targetPosition_, map);
            if (currentPath_.empty()) {
                map.addSystemMessage(L"Unable to find path to target.");
                ai_->CombatChangeState(std::make_unique<CombatIdleState>(ai_));
                return;
            }
        }

        if (unit->isReadyToMove(currentTime)) {
            types::Position nextPos = currentPath_.back();
            currentPath_.pop_back();
            unit->moveTo(nextPos);
            unit->updateLastMoveTime(currentTime);

            if (currentPath_.empty()) {
                // 목적지 도달
                ai_->CombatChangeState(std::make_unique<CombatIdleState>(ai_));
            }
        }
    }

    // 공격 상태 구현
    AttackingState::AttackingState(CombatUnitAI* ai, Unit* target)
        : ai_(ai)
        , target_(target)
        , lastAttackTime_(std::chrono::milliseconds(0)) {}

    void AttackingState::update(
        Unit* unit,
        core::Map& map,
        std::chrono::milliseconds currentTime
    ) {
        if (!target_ || target_->getHealth() <= 0) {
            // 타겟이 죽었거나 없어진 경우
            ai_->CombatChangeState(std::make_unique<CombatIdleState>(ai_));
            return;
        }

        if (!isInAttackRange(unit, target_)) {
            // 타겟이 공격 범위를 벗어난 경우 추적 상태로 전환
            ai_->CombatChangeState(std::make_unique<PursuingState>(ai_, target_));
            return;
        }

        if (canAttack(unit, currentTime)) {
            // 공격 실행
            int damage = unit->getAttackPower();

            // 메시지 생성
            std::wstring message;
            if (unit->getCamp() == types::Camp::ArtLadies) {
                message = L"Our " + getUnitTypeName(unit->getType()) +
                    L" attacks enemy for " + std::to_wstring(damage) + L" damage!";
            }
            else {
                message = L"Enemy " + getUnitTypeName(unit->getType()) +
                    L" attacks us for " + std::to_wstring(damage) + L" damage!";
            }
            map.addSystemMessage(message);

            // TODO: target_->takeDamage(damage) 구현 필요
            lastAttackTime_ = currentTime;
        }
    }

    std::wstring AttackingState::getUnitTypeName(types::UnitType type) const {
        switch (type) {
        case types::UnitType::Soldier:    return L"Soldier";
        case types::UnitType::Fremen:     return L"Fremen";
        case types::UnitType::Fighter:    return L"Fighter";
        case types::UnitType::HeavyTank:  return L"Heavy Tank";
        default:                          return L"Unit";
        }
    }

    // 추적 상태 구현
    PursuingState::PursuingState(CombatUnitAI* ai, Unit* target)
        : ai_(ai)
        , target_(target)
        , lastPathUpdateTime_(std::chrono::milliseconds(0)) {}

    void PursuingState::update(
        Unit* unit,
        core::Map& map,
        std::chrono::milliseconds currentTime
    ) {
        if (!target_ || target_->getHealth() <= 0) {
            ai_->CombatChangeState(std::make_unique<CombatIdleState>(ai_));
            return;
        }

        // 타겟이 공격 범위 안에 들어오면 공격 상태로 전환
        if (isInAttackRange(unit, target_)) {
            ai_->CombatChangeState(std::make_unique<AttackingState>(ai_, target_));
            return;
        }

        // 주기적으로 경로 업데이트
        if (currentPath_.empty() ||
            (currentTime - lastPathUpdateTime_).count() > 1000) {  // 1초마다 경로 갱신
            currentPath_ = findPath(
                unit->getPosition(),
                target_->getPosition(),
                map
            );
            lastPathUpdateTime_ = currentTime;
        }

        if (!currentPath_.empty() && unit->isReadyToMove(currentTime)) {
            types::Position nextPos = currentPath_.back();
            currentPath_.pop_back();
            unit->moveTo(nextPos);
            unit->updateLastMoveTime(currentTime);
        }
    }

    // 순찰 상태 구현
    PatrollingState::PatrollingState(
        CombatUnitAI* ai,
        const types::Position& from,
        const types::Position& to
    )
        : ai_(ai)
        , fromPosition_(from)
        , toPosition_(to)
        , currentTarget_(to) {}

    void PatrollingState::update(
        Unit* unit,
        core::Map& map,
        std::chrono::milliseconds currentTime
    ) {
        // 적 발견 시 추적으로 전환
        const auto& quadTree = map.getUnitManager().getQuadTree();
        std::vector<const entity::Unit*> nearbyUnits;

        types::Position pos = unit->getPosition();
        int sightRange = unit->getSightRange();

        quadTree.queryRange(
            pos.column - sightRange,  // qx
            pos.row - sightRange,     // qy
            sightRange * 2,           // qw
            sightRange * 2,           // qh
            nearbyUnits               // results
        );

        for (const auto* nearbyUnit : nearbyUnits) {
            if (nearbyUnit->getCamp() != unit->getCamp()) {
                ai_->CombatChangeState(std::make_unique<PursuingState>(ai_, const_cast<Unit*>(nearbyUnit)));
                return;
            }
        }

        if (currentPath_.empty()) {
            currentPath_ = findPath(unit->getPosition(), currentTarget_, map);
        }

        if (!currentPath_.empty() && unit->isReadyToMove(currentTime)) {
            types::Position nextPos = currentPath_.back();
            currentPath_.pop_back();
            unit->moveTo(nextPos);
            unit->updateLastMoveTime(currentTime);

            if (currentPath_.empty()) {
                // 목적지에 도달하면 다음 목적지로 전환
                currentTarget_ = (currentTarget_ == toPosition_) ? fromPosition_ : toPosition_;
            }
        }
    }
}