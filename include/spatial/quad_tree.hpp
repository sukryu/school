#pragma once
#include <vector>
#include <memory>
#include "utils/types.hpp"
#include "entity/unit.hpp"

namespace dune {
    namespace spatial {

        /**
         * @brief 쿼드트리 노드 클래스
         * 각 노드는 사각형 영역을 대표하며, 해당 영역 내의 객체(유닛)을 관리합니다.
         */
        class QuadTree {
        public:
            /**
             * @brief 생성자
             * @param x 루트 노드 영역의 왼쪽 상단 x좌표(여기서는 column)
             * @param y 루트 노드 영역의 왼쪽 상단 y좌표(여기서는 row)
             * @param width 루트 노드 영역의 너비
             * @param height 루트 노드 영역의 높이
             * @param maxObjectsPerNode 한 노드가 유지할 수 있는 최대 객체 수
             */
            QuadTree(int x, int y, int width, int height, int maxObjectsPerNode = 10)
                : x_(x), y_(y), width_(width), height_(height),
                maxObjectsPerNode_(maxObjectsPerNode) {}

            /**
             * @brief 객체(유닛)을 삽입합니다.
             * @param unit 삽입할 유닛의 포인터
             */
            void insert(const entity::Unit* unit);

            /**
             * @brief 특정 사각형 범위에 해당하는 객체 목록을 가져옵니다.
             * @param qx 쿼리 영역의 왼쪽 상단 x
             * @param qy 쿼리 영역의 왼쪽 상단 y
             * @param qw 쿼리 영역의 너비
             * @param qh 쿼리 영역의 높이
             * @param results 결과를 담을 벡터(범위 내 객체의 포인터)
             */
            void queryRange(int qx, int qy, int qw, int qh, std::vector<const entity::Unit*>& results) const;

            /**
             * @brief 쿼드트리에서 특정 유닛을 제거합니다.
             * @param unit 제거할 유닛 포인터
             * @return true 제거 성공, false 해당 유닛 미존재
             */
            bool remove(const entity::Unit* unit);

        private:
            int x_, y_, width_, height_;
            int maxObjectsPerNode_;

            std::vector<const entity::Unit*> objects_;
            std::unique_ptr<QuadTree> children_[4]; // 자식 노드 (분할 시 생성)

            bool isLeaf() const {
                return !children_[0]; // 첫 번째 자식이 없으면 Leaf
            }

            /**
             * @brief 노드를 4개 자식으로 분할
             */
            void split();

            /**
             * @brief 객체가 어느 자식 노드로 들어갈지 판단
             * @param unit 객체 포인터
             * @return -1이면 자식 노드에 들어갈 필요 없음(현재 노드에 속함), 0~3이면 해당 Quadrant 인덱스
             */
            int getQuadrant(const entity::Unit* unit) const;

            /**
             * @brief 현재 노드 영역과 쿼리 영역이 겹치는지 확인
             */
            bool intersects(int qx, int qy, int qw, int qh) const;
        };

    } // namespace spatial
} // namespace dune
