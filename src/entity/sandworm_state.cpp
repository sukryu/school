#include "entity/sandworm_state.hpp"
#include "entity/sandworm_ai.hpp"
#include "core/map.hpp"
#include "utils/utils.hpp"
#include "utils/types.hpp"
#include "core/map.hpp"
#include <queue>
#include <unordered_set>
#include <array>

namespace dune::entity {
    bool SandwormState::isValidTarget(const Unit* target) const {
        if (!target) return false;
        auto targetType = target->getType();
        return targetType != types::UnitType::Sandworm &&
            targetType != types::UnitType::HeavyTank;
    }

    void SandwormState::changeState(SandwormAI* ai, std::unique_ptr<SandwormState> newState) {
        if (ai) {
            ai->changeState(std::move(newState));
        }
    }

    std::vector<types::Position> SandwormState::findPath(
        const types::Position& start,
        const types::Position& goal,
        const dune::core::Map& map
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



    types::Position SandwormState::findNearestPrey(const Unit* sandworm, const dune::core::Map& map) const {
        types::Position nearestPos = sandworm->getPosition();
        int minDistance = std::numeric_limits<int>::max();

        std::vector<const Unit*> nearbyUnits;
        map.getUnitManager().getQuadTree().queryRange(
            sandworm->getPosition().column - 10,
            sandworm->getPosition().row - 10,
            20, 20,
            nearbyUnits
        );

        for (const auto* unit : nearbyUnits) {
            if (!unit || !isValidTarget(unit)) continue;

            int distance = utils::manhattanDistance(
                sandworm->getPosition(),
                unit->getPosition()
                );

            if (distance < minDistance) {
                minDistance = distance;
                nearestPos = unit->getPosition();
            }
        }

        return nearestPos;
    }

    types::Position SandwormState::findSuitableExcretionSpot(
        const types::Position& currentPos, const dune::core::Map& map
    ) const {
        std::vector<types::Position> candidates;

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                types::Position pos{
                    currentPos.row + dy,
                    currentPos.column + dx,
                };

                if (!pos.is_valid()) continue;

                const auto& terrain = map.getTerrainManager().getTerrain(pos);
                if (terrain.getType() == types::TerrainType::Desert &&
                    !map.getEntityAt<Unit>(pos) &&
                    !map.getEntityAt<Building>(pos)) {
                    candidates.push_back(pos);
                }
            }
        }

        if (candidates.empty()) {
            return currentPos;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, candidates.size() - 1);
        return candidates[dis(gen)];
    }

    void HuntingState::update(Unit* sandworm, dune::core::Map& map, std::chrono::milliseconds currentTime) {
        if (!sandworm->isReadyToMove(currentTime)) return;

        if (currentPath_.empty() || currentTime - lastPathUpdate_ >= PATH_UPDATE_INTERVAL) {
            types::Position targetPos = findNearestPrey(sandworm, map);

            if (targetPos != sandworm->getPosition()) {
                currentPath_ = findPath(sandworm->getPosition(), targetPos, map);
                lastPathUpdate_ = currentTime;
            }
        }

        // 경로를 따라 이동
        if (!currentPath_.empty()) {
            types::Position nextPos = currentPath_.back();
            currentPath_.pop_back();

            if (isValidMovePosition(nextPos, map)) {
                if (Unit* prey = map.getEntityAt<Unit>(nextPos)) {
                    if (isValidTarget(prey)) {
                        sandworm->consumeTarget();
                        map.addSystemMessage(L"Sandworm successfully caught its prey!");
                        map.removeUnit(prey);
                        sandworm->getAI()->changeState(std::make_unique<DigestingState>(sandworm->getAI()));
                        return;
                    }
                }

                sandworm->moveTo(nextPos);
                sandworm->updateLastMoveTime(currentTime);
            }
        }
    }

    bool HuntingState::isValidMovePosition(const types::Position& pos, const dune::core::Map & map) const {
        if (!pos.is_valid()) return false;

        const auto& terrain = map.getTerrainManager().getTerrain(pos);
        return terrain.getType() != types::TerrainType::Rock &&
            !map.getEntityAt<Building>(pos);
    }

    void DigestingState::update(Unit* sandworm, dune::core::Map& map, std::chrono::milliseconds currentTime) {
        // 소화 시작 시간 초기화
        if (digestStartTime_.count() == 0) {
            digestStartTime_ = currentTime;
            map.addSystemMessage(L"Sandworm is digesting its prey...");
        }

        // 소화 완료 체크
        if (currentTime - digestStartTime_ >= DIGESTION_TIME) {
            if (sandworm->shouldExcrete()) {
                map.addSystemMessage(L"Sandworm is preparing to create spice...");
                if (auto ai = sandworm->getAI()) { // AI가 nullptr이 아닌지 확인
                    ai->changeState(std::make_unique<BurrowingState>(sandworm->getAI()));
                }
                else {
                    map.addSystemMessage(L"Error: Sandworm AI not initialized.");
                }
            }
            else {
                map.addSystemMessage(L"Sandworm returns to hunting.");
                if (auto ai = sandworm->getAI()) {
                    ai->changeState(std::make_unique<HuntingState>(sandworm->getAI()));
                }
                else {
                    map.addSystemMessage(L"Error: Sandworm AI not initialized.");
                }
            }
        }
    }

    void BurrowingState::update(Unit* sandworm, dune::core::Map& map, std::chrono::milliseconds currentTime) {
        // 굴파기 시작 시간 초기화
        if (burrowStartTime_.count() == 0) {
            burrowStartTime_ = currentTime;
            burrowTarget_ = findSuitableExcretionSpot(sandworm->getPosition(), map);
            map.addSystemMessage(L"Sandworm is burrowing...");
        }

        // 굴파기 완료 체크
        if (currentTime - burrowStartTime_ >= BURROW_TIME) {
            // 스파이스 생성
            sandworm->excrete();
            map.setTerrain(burrowTarget_, types::TerrainType::Spice);
            map.addSystemMessage(L"Sandworm has created a new spice field!");

            // 사냥 상태로 전환
            sandworm->getAI()->changeState(std::make_unique<HuntingState>(sandworm->getAI()));
        }
    }
}
