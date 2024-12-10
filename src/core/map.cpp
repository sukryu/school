#include "core/map.hpp"
#include "ui/window/message_window.hpp"
#include "utils/utils.hpp"
#include <limits>

namespace dune {
    namespace core {

        Map::Map(int width, int height, ui::MessageWindow* messageWindow)
            : width_(width)
            , height_(height)
            , terrainManager_(width, height)
            , messageWindow_(messageWindow) {
            // 필요한 초기화 작업을 수행합니다.
        }

        void Map::update(std::chrono::milliseconds currentTime) {
            // 각 유닛을 업데이트합니다.
           for (const auto& entry : unitManager_.getUnits()) {
                // entry는 pair<const types::Position, std::unique_ptr<Unit>> 이므로
                // 실제 유닛 객체는 entry.second-> 를 통해 접근해야 합니다.
                const auto& unitPtr = entry.second; // std::unique_ptr<Unit>에 대한 참조
                // unitPtr는 unique_ptr<Unit>이므로 unitPtr->getType() 형태로 호출 가능
                switch (unitPtr->getType()) {
                    case types::UnitType::Sandworm:
                        if (auto* sandwormAI = unitPtr->getSandwormAI()) {
                            sandwormAI->update(unitPtr.get(), *this, currentTime);
                        }
                        else {
                            addSystemMessage(L"Cannot Initialize S AI");
                        }
                        break;

                    case types::UnitType::Harvester:
                        if (auto* harvesterAI = unitPtr->getHarvesterAI()) {
                            addSystemMessage(L"[DEBUG] Updating harvester AI");
                            harvesterAI->update(unitPtr.get(), *this, currentTime);
                        }
                        else {
                            addSystemMessage(L"Cannot Initialize H AI");
                        }
                        break;

                    case types::UnitType::Soldier:
                    case types::UnitType::Fremen:
                    case types::UnitType::Fighter:
                    case types::UnitType::HeavyTank:
                        if (auto* combatAI = unitPtr->getCombatUnitAI()) {
                            combatAI->update(*this, currentTime);
                        }
                        else {
                            addSystemMessage(L"Cannot Initialize C AI");
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        void Map::addUnit(std::unique_ptr<Unit> unit) {
            unitManager_.addUnit(std::move(unit));
        }

        void Map::addBuilding(std::unique_ptr<Building> building) {
            buildingManager_.addBuilding(std::move(building));
        }

        void Map::setTerrain(const types::Position& position, types::TerrainType type) {
            terrainManager_.setTerrain(position, type);
        }

        types::Position Map::findNearestUnit(const types::Position& fromPosition, types::UnitType excludeType) {
            types::Position nearestPosition = fromPosition;
            int minDistance = std::numeric_limits<int>::max();

            int searchRadius = 10; // 초기 검색 반경
            std::vector<const entity::Unit*> candidates;

            while (true) {
                // 검색 반경 내 유닛 쿼리
                unitManager_.getQuadTree().queryRange(
                    fromPosition.column - searchRadius,
                    fromPosition.row - searchRadius,
                    searchRadius * 2,
                    searchRadius * 2,
                    candidates);

                for (const auto* unit : candidates) {
                    // 제외할 타입 확인
                    if (unit->getType() == excludeType) {
                        continue;
                    }

                    // 샌드웜 타겟 적합성 확인
                    if (!isValidSandwormTarget(unit)) {
                        continue;
                    }

                    // 거리 계산
                    int distance = utils::manhattanDistance(fromPosition, unit->getPosition());

                    // 최소 거리 갱신
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearestPosition = unit->getPosition();
                    }
                }

                // 최소 거리 갱신이 되었거나, 검색 반경이 맵 크기를 초과하면 종료
                if (minDistance < searchRadius || searchRadius > std::max(width_, height_)) {
                    break;
                }

                // 검색 반경 확장
                searchRadius *= 2;
                candidates.clear();
            }

            return nearestPosition;
        }


        void Map::removeUnit(Unit* unit) {
            if (unit) {
                unitManager_.removeUnit(unit);
            }
        }

        void Map::damageBuildingAt(const types::Position& position, int damage) {
            if (auto* building = buildingManager_.getBuildingAt(position)) {
                building->takeDamage(damage);
                if (building->isDestroyed()) {
                    addSystemMessage(L"The building was destroyed!");
                }
            }
        }

        void Map::removeDestroyedBuildings() {
            buildingManager_.removeDestroyedBuildings();
        }

        void Map::addSystemMessage(const std::wstring& message) {
            if (messageWindow_) {
                messageWindow_->addMessage(message);
            }
        }

        /**
         * @brief 샌드웜을 업데이트합니다.
         * @param sandworm 업데이트할 샌드웜 유닛.
         * @param currentTime 현재 시간.
         */
        void Map::updateSandworm(Unit* sandworm, std::chrono::milliseconds currentTime) {
            if (!sandworm->isReadyToMove(currentTime)) {
                return;
            }

            // 가장 가까운 유닛을 찾습니다.
            types::Position targetPosition = findNearestUnit(sandworm->getPosition(), types::UnitType::Sandworm);

            // 대상 위치로 이동합니다.
            if (targetPosition != sandworm->getPosition()) {
                types::Position newPosition = calculateSandwormMove(sandworm, targetPosition);

                sandworm->moveTo(newPosition);

                // 새 위치에서 유닛 확인
                if (Unit* targetUnit = getEntityAt<Unit>(newPosition)) {
                    if (isValidSandwormTarget(targetUnit)) {
                        // 유닛을 잡아먹습니다.
                        sandworm->consumeTarget();
                        addSystemMessage(L"Sandworm caught a " + getUnitTypeName(targetUnit->getType()) + L"!");
                        removeUnit(targetUnit);

                        // 스파이스 생성 여부 확인
                        if (sandworm->shouldExcrete()) {
                            sandworm->excrete();
                            setTerrain(newPosition, types::TerrainType::Spice);
                            addSystemMessage(L"The sandworm left behind some spice.");
                        }
                    }
                }
            }

            sandworm->updateLastMoveTime(currentTime);
        }

        types::Position Map::calculateSandwormMove(const Unit* sandworm, const types::Position& targetPosition) {
            types::Position currentPosition = sandworm->getPosition();
            types::Position nextPosition = currentPosition;

            // 수평 거리와 수직 거리 계산
            int dx = targetPosition.column - currentPosition.column;
            int dy = targetPosition.row - currentPosition.row;

            // 더 먼 방향으로 이동 시도
            if (abs(dx) > abs(dy)) {
                // 수평 이동 시도
                nextPosition.column += (dx > 0) ? 1 : -1;
                // 바위를 만나면 수직 이동 시도
                if (terrainManager_.getTerrain(nextPosition).getType() == types::TerrainType::Rock) {
                    nextPosition.column = currentPosition.column;
                    nextPosition.row += (dy > 0) ? 1 : -1;
                }
            } else {
                // 수직 이동 시도
                nextPosition.row += (dy > 0) ? 1 : -1;
                // 바위를 만나면 수평 이동 시도
                if (terrainManager_.getTerrain(nextPosition).getType() == types::TerrainType::Rock) {
                    nextPosition.row = currentPosition.row;
                    nextPosition.column += (dx > 0) ? 1 : -1;
                }
            }

            return nextPosition;
        }

        bool Map::isValidSandwormTarget(const Unit* target) const {
            if (!target) return false;
            auto targetType = target->getType();
            return targetType != types::UnitType::Sandworm &&
                   targetType != types::UnitType::HeavyTank;
        }

        std::wstring Map::getUnitTypeName(types::UnitType type) const {
            switch (type) {
            case types::UnitType::Harvester: return L"Harvester";
            case types::UnitType::Fremen:    return L"Fremen";
            case types::UnitType::Soldier:   return L"Soldier";
            case types::UnitType::Fighter:   return L"Fighter";
            case types::UnitType::HeavyTank: return L"HeavyTank";
            default:                         return L"unit";
            }
        }

    } // namespace core
} // namespace dune
