#pragma once
#include "../core/entity.hpp"
#include "../utils/types.hpp"
#include "entity/unit.hpp"
#include "spatial/quad_tree.hpp"
#include <memory>
#include <unordered_map>
#include <chrono>

namespace dune {
    namespace managers {
        /**
         * @brief 게임 내 모든 유닛을 관리하는 클래스입니다.
         */
        class UnitManager {
        public:
            using Unit = dune::entity::Unit;

            UnitManager();
            /**
             * @brief 유닛을 추가합니다.
             * @param unit 추가할 유닛의 unique_ptr.
             */
            void addUnit(std::unique_ptr<Unit> unit);

            /**
             * @brief 특정 위치에 있는 유닛을 반환합니다.
             * @param position 확인할 위치.
             * @return Unit* 해당 위치에 있는 유닛 포인터 (없으면 nullptr).
             */
            Unit* getUnitAt(const types::Position& position);
            const Unit* getUnitAt(const types::Position& position) const;

            /**
             * @brief 특정 유닛을 제거합니다.
             * @param unit 제거할 유닛의 포인터.
             */
            void removeUnit(Unit* unit);


            using UnitMap = std::unordered_map<types::Position, std::unique_ptr<Unit>>;
            /**
             * @brief 현재 존재하는 모든 유닛을 반환합니다.
             * @return unitsByPosition_에 대한 const 참조.
             */
            const UnitMap& getUnits() const;

            const dune::spatial::QuadTree& getQuadTree() const { return quadTree_; }

        private:
            // 위치를 키로 하여 유닛을 관리합니다.
            UnitMap unitsByPosition_;

            // 맵 전체를 커버하는 루트 노드
            dune::spatial::QuadTree quadTree_;
        };

    } // namespace managers
} // namespace dune
