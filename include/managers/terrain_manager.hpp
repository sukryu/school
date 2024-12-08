#pragma once
#include "../core/entity.hpp"
#include "../utils/types.hpp"
#include "entity/terrain.hpp"
#include <vector>

namespace dune {
    namespace managers {

        /**
         * @brief 게임 맵 전체의 지형 정보를 관리하는 클래스입니다.
         */
        class TerrainManager {
        public:

            using Terrain = dune::entity::Terrain;

            /**
             * @brief TerrainManager 클래스의 생성자입니다.
             * @param width 맵의 가로 크기.
             * @param height 맵의 세로 크기.
             */
            TerrainManager(int width, int height);

            /**
             * @brief 특정 위치의 지형 정보를 반환합니다.
             * @param position 확인할 위치.
             * @return const Terrain& 해당 위치의 지형 객체.
             */
            const Terrain& getTerrain(const types::Position& position) const;

            /**
             * @brief 특정 위치의 지형 타입을 설정합니다.
             * @param position 설정할 위치.
             * @param type 지형 타입.
             */
            void setTerrain(const types::Position& position, types::TerrainType type);

            /**
             * @brief 위치가 맵 범위 내의 유효한 위치인지 확인합니다.
             * @param position 확인할 위치.
             * @return true 유효한 위치이면 true.
             * @return false 그렇지 않으면 false.
             */
            bool isValidPosition(const types::Position& position) const;

        private:
            int width_;
            int height_;
            std::vector<std::vector<Terrain>> terrainMap_;
        };

    } // namespace managers
} // namespace dune
