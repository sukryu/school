#include "entity/harvester_state.hpp"
#include "core/map.hpp"
#include "utils/utils.hpp"
#include <queue>
#include <unordered_set>
#include <array>

namespace dune::entity {
    
    std::vector<types::Position> HarvesterState::findPath(
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

    bool HarvesterState::isValidPosition(
        const types::Position& pos,
        const core::Map& map
    ) const {
        if (!pos.is_valid()) return false;

        const auto& terrain = map.getTerrainManager().getTerrain(pos);
        return terrain.isWalkable() && !map.getEntityAt<Building>(pos);
    }

    // 대기중인 상태.
    IdleState::IdleState(HarvesterAI* ai)
        : ai_(ai) {}

    void IdleState::update(
        Unit* unit,
        core::Map& map,
        std::chrono::milliseconds currentTime
    ) {
        // 대기 상태에서는 특별한 동작 없음
    }

    // 이동 중인 상태 클래스 부분
    MovingState::MovingState(HarvesterAI* ai, const types::Position& target)
        : ai_(ai), targetPosition_(target) {};

    void
    MovingState::update(
        Unit* harvester,
        core::Map& map,
        std::chrono::milliseconds currentTime
    ) {
        map.addSystemMessage(L"[DEBUG] MovingToHarvestState::update");
        if (currentPath_.empty()) {
            currentPath_ = findPath(harvester->getPosition(), targetPosition_, map);
            if (currentPath_.empty()) {
                map.addSystemMessage(L"[DEBUG] Unable to find path to spice field");
                ai_->changeState(std::make_unique<IdleState>(ai_));
                return;
            }
        }

        if (harvester->isReadyToMove(currentTime)) {
            types::Position nextPos = currentPath_.back();
            currentPath_.pop_back();
            harvester->moveTo(nextPos);
            harvester->updateLastMoveTime(currentTime);
            map.addSystemMessage(L"[DEBUG] Moving to next position: " +
                std::to_wstring(nextPos.row) + L"," + std::to_wstring(nextPos.column));

            if (currentPath_.empty()) {
                // 목적지 도달
                map.addSystemMessage(L"[DEBUG] Reached spice field, changing to HarvestingState");
                ai_->changeState(std::make_unique<IdleState>(ai_));
            }
        }
    }

    // 수확 위치로 이동하는 상태 클래스
    MovingToHarvestState::MovingToHarvestState(HarvesterAI* ai, const types::Position& spicePos)
        : ai_(ai), spicePosition_(spicePos) {}

    void 
    MovingToHarvestState::update(
        Unit* harvester,
        core::Map& map,
        std::chrono::milliseconds currentTime
    ) {
        if (currentPath_.empty()) {
            currentPath_ = findPath(harvester->getPosition(), spicePosition_, map);
            if (currentPath_.empty()) {
                map.addSystemMessage(L"Unable to find path to spice field.");
                ai_->changeState(std::make_unique<IdleState>(ai_));
                return;
            }
        }

        if (harvester->isReadyToMove(currentTime)) {
            types::Position nextPos = currentPath_.back();
            currentPath_.pop_back();
            harvester->moveTo(nextPos);
            harvester->updateLastMoveTime(currentTime);

            if (currentPath_.empty()) {
                // 스파이스 위치 도달
                ai_->changeState(std::make_unique<HarvestingState>(ai_));
            }
        }
    }

    // 수확 중인 상태 클래스
    HarvestingState::HarvestingState(HarvesterAI* ai)
        : ai_(ai), harvestStartTime_(std::chrono::milliseconds(0)) {}

    void HarvestingState::update(
            Unit* unit,
            core::Map& map,
            std::chrono::milliseconds currentTime
        ) {
        if (harvestStartTime_.count() == 0) {
            harvestStartTime_ = currentTime;
            map.addSystemMessage(L"Harvester began collecting spice...");
        }

        if (currentTime - harvestStartTime_ >= HARVEST_TIME) {
            // 수확량 결정 (2~4)
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(MIN_HARVEST, MAX_HARVEST);
            int amount = dis(gen);

            ai_->setSpiceAmount(amount);
            map.addSystemMessage(L"Collected " + std::to_wstring(amount) + L" spice.");

            // 본진으로 돌아가기
            ai_->changeState(std::make_unique<ReturningState>(ai_));
        }
    }

    // 본진으로 돌아가는 상태 클래스.
    ReturningState::ReturningState(HarvesterAI* ai) : ai_(ai) {};

    void ReturningState::update(
        Unit* harvester,
        core::Map& map,
        std::chrono::milliseconds currentTime
    ) {
        if (currentPath_.empty()) {
            currentPath_ = findPath(harvester->getPosition(),
                ai_->getBasePosition(), map);
            if (currentPath_.empty()) {
                map.addSystemMessage(L"Unable to find path back to base.");
                ai_->changeState(std::make_unique<IdleState>(ai_));
                return;
            }
        }

        if (harvester->isReadyToMove(currentTime)) {
            types::Position nextPos = currentPath_.back();
            currentPath_.pop_back();
            harvester->moveTo(nextPos);
            harvester->updateLastMoveTime(currentTime);

            if (currentPath_.empty()) {
                // 본진 도착 - 시스템 메시지만 전송
                int spiceAmount = ai_->getSpiceAmount();
                map.addSystemMessage(L"Harvester returned with " +
                    std::to_wstring(spiceAmount) +
                    L" spice.");

                // 스파이스 양을 0으로 리셋
                ai_->setSpiceAmount(0);

                // 대기 상태로 전환
                ai_->changeState(std::make_unique<IdleState>(ai_));
            }
        }
    }
}