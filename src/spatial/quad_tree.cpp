#include "spatial/quad_tree.hpp"
#include <algorithm>

namespace dune {
    namespace spatial {

        void QuadTree::split() {
            int halfW = width_ / 2;
            int halfH = height_ / 2;

            // 상-left
            children_[0] = std::make_unique<QuadTree>(x_, y_, halfW, halfH, maxObjectsPerNode_);
            // 상-right
            children_[1] = std::make_unique<QuadTree>(x_ + halfW, y_, width_ - halfW, halfH, maxObjectsPerNode_);
            // 하-left
            children_[2] = std::make_unique<QuadTree>(x_, y_ + halfH, halfW, height_ - halfH, maxObjectsPerNode_);
            // 하-right
            children_[3] = std::make_unique<QuadTree>(x_ + halfW, y_ + halfH, width_ - halfW, height_ - halfH, maxObjectsPerNode_);

            // 기존 objects_에 있는 객체들을 재분배
            auto it = objects_.begin();
            while (it != objects_.end()) {
                int quadrant = getQuadrant(*it);
                if (quadrant != -1) {
                    children_[quadrant]->insert(*it);
                    it = objects_.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        int QuadTree::getQuadrant(const entity::Unit* unit) const {
            // unit 포인터의 유효성 검사
            if (!unit) {
                throw std::invalid_argument("Invalid unit pointer passed to QuadTree::getQuadrant.");
            }

            types::Position pos = unit->getPosition();
            int mx = x_ + width_ / 2;
            int my = y_ + height_ / 2;

            bool top = (pos.row < my);
            bool left = (pos.column < mx);

            // Quadrant 인덱스: 0=상-left, 1=상-right, 2=하-left, 3=하-right
            if (top && left) return 0;
            else if (top && !left) return 1;
            else if (!top && left) return 2;
            else if (!top && !left) return 3;

            return -1; // 해당 노드에 완전히 속하지 않으면 -1
        }

        bool QuadTree::intersects(int qx, int qy, int qw, int qh) const {
            // 두 사각형 (x_, y_, width_, height_) vs (qx, qy, qw, qh) 겹침 여부
            bool noOverlap = (x_ + width_ < qx) || (x_ > qx + qw) ||
                (y_ + height_ < qy) || (y_ > qy + qh);
            return !noOverlap;
        }

        void QuadTree::insert(const entity::Unit* unit) {
            // 만약 자식이 있으면 어느 자식으로 들어갈지 확인
            if (!isLeaf()) {
                int quadrant = getQuadrant(unit);
                if (quadrant != -1) {
                    children_[quadrant]->insert(unit);
                    return;
                }
            }

            // 현재 노드에 추가
            objects_.push_back(unit);

            // 임계값 초과 시 분할
            if (isLeaf() && (int)objects_.size() > maxObjectsPerNode_ && width_ > 1 && height_ > 1) {
                split();
            }
        }

        void QuadTree::queryRange(int qx, int qy, int qw, int qh, std::vector<const entity::Unit*>& results) const {
            if (!intersects(qx, qy, qw, qh)) {
                // 전혀 영역 겹치지 않으므로 검색 필요 없음
                return;
            }

            // 현재 노드에 속한 객체 중에서 범위에 들어가는 것 찾기
            for (auto* unit : objects_) {
                types::Position pos = unit->getPosition();
                if (pos.column >= qx && pos.column <= qx + qw &&
                    pos.row >= qy && pos.row <= qy + qh) {
                    results.push_back(unit);
                }
            }

            // 자식이 있으면 재귀 탐색
            if (!isLeaf()) {
                for (int i = 0; i < 4; ++i) {
                    children_[i]->queryRange(qx, qy, qw, qh, results);
                }
            }
        }

        bool QuadTree::remove(const entity::Unit* unit) {
            // 만약 자식이 있다면, 이 유닛이 어느 자식에 속하는지 확인
            if (!isLeaf()) {
                int quadrant = getQuadrant(unit);
                if (quadrant != -1) {
                    // 자식 노드로 내려가서 제거 시도
                    return children_[quadrant]->remove(unit);
                }
            }

            // 현재 노드 objects_에서 유닛 제거 시도
            // 동일한 포인터를 가진 유닛을 찾는다.
            for (auto it = objects_.begin(); it != objects_.end(); ++it) {
                if (*it == unit) {
                    objects_.erase(it);
                    return true;
                }
            }

            // 현재 노드와 자식 노드 모두에서 찾지 못한 경우
            return false;
        }

    } // namespace spatial
} // namespace dune
