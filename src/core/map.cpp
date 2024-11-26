#include "map.hpp"
#include "../ui/window/message_window.hpp"
#include "../utils/utils.hpp"
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
            for (const auto& unit : unitManager_.getUnits()) {
                switch (unit->getType()) {
                case types::UnitType::Sandworm:
                    updateSandworm(unit.get(), currentTime);
                    break;
                // 다른 유닛 타입의 업데이트 로직을 추가합니다.
                default:
                    // 일반 유닛의 기본 동작 (이동, 전투 등)
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

            for (const auto& unit : unitManager_.getUnits()) {
                // 제외할 타입의 유닛은 건너뜁니다.
                if (unit->getType() == excludeType) {
                    continue;
                }

                // 잡아먹을 수 없는 유닛은 제외
                if (!isValidSandwormTarget(unit.get())) {
                    continue;
                }

                types::Position unitPosition = unit->getPosition();
                int distance = utils::manhattanDistance(unitPosition, fromPosition);

                if (distance < minDistance) {
                    minDistance = distance;
                    nearestPosition = unitPosition;
                }
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
