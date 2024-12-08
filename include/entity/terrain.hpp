#pragma once
#include "../core/entity.hpp"
#include "../utils/types.hpp"
#include <vector>

namespace dune {
    namespace entity {
        /**
         * @brief 지형을 나타내는 클래스입니다.
         */
        class Terrain : public core::Entity {
        public:
            /**
             * @brief Terrain 클래스의 생성자입니다.
             * @param type 지형의 타입.
             */
            explicit Terrain(types::TerrainType type = types::TerrainType::Desert, const types::Position& position = { 0,0 });

            // Entity 인터페이스 구현
            wchar_t getRepresentation() const override;
            int getColor() const override;
            void printInfo() const override;
            types::Position getPosition() const override { return position_; }
            void move(types::Direction direction) override {} // 지형은 이동 불가

            // Terrain 고유 메서드

            /**
             * @brief 해당 지형 위에 건물을 건설할 수 있는지 확인합니다.
             * @return true 건설 가능하면 true.
             * @return false 건설 불가능하면 false.
             */
            bool isBuildable() const;

            /**
             * @brief 해당 지형을 유닛이 통과할 수 있는지 확인합니다.
             * @return true 통과 가능하면 true.
             * @return false 통과 불가능하면 false.
             */
            bool isWalkable() const;

            /**
             * @brief 해당 지형에서 스파이스를 채취할 수 있는지 확인합니다.
             * @return true 채취 가능하면 true.
             * @return false 채취 불가능하면 false.
             */
            bool canHarvestSpice() const;

            /**
             * @brief 지형의 타입을 반환합니다.
             * @return types::TerrainType 지형 타입.
             */
            types::TerrainType getType() const { return type_; }

        private:
            types::TerrainType type_;
            types::Position position_;
        };
    } // namespace entity
} // namespace dune
