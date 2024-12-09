#pragma once
#include "../utils/types.hpp"
#include <chrono>
#include <vector>
#include <string>
#include <random>

// 전방 선언
namespace dune {
    namespace core { class Map; }
    namespace entity {
        class HarvesterAI;
        class Unit;
        class Building;
    }
}

namespace dune::entity {
    /**
     * @brief 하베스터의 기본 상태 클래스입니다.
     */
    class HarvesterState {
    public:
        virtual ~HarvesterState() = default;

        /**
         * @brief 상태 업데이트를 처리합니다.
         */
        virtual void update(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime) = 0;

        /**
         * @brief 현재 상태의 이름을 반환합니다.
         */
        virtual std::wstring getStateName() const = 0;

    protected:
        /**
         * @brief A* 알고리즘을 사용하여 경로를 찾습니다.
         */
        std::vector<types::Position> findPath(
            const types::Position& start,
            const types::Position& goal,
            const core::Map& map) const;

        /**
         * @brief 해당 위치로 이동 가능한지 확인합니다.
         */
        bool isValidPosition(const types::Position& pos, const core::Map& map) const {
            if (!pos.is_valid()) return false;

            const auto& terrain = map.getTerrainManager().getTerrain(pos);
            return terrain.isWalkable() && !map.getEntityAt<Building>(pos);
        }
    };

    /**
     * @brief 대기 상태 클래스입니다.
     */
    class IdleState : public HarvesterState {
    public:
        explicit IdleState(HarvesterAI* ai) : ai_(ai) {}

        void update(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime) override {
            // 대기 상태에서는 특별한 동작 없음
        }

        std::wstring getStateName() const override { return L"Idle"; }

    private:
        HarvesterAI* ai_;
    };

    /**
     * @brief 이동 중인 상태 클래스입니다.
     */
    class MovingState : public HarvesterState {
    public:
        MovingState(HarvesterAI* ai, const types::Position& target)
            : ai_(ai), targetPosition_(target) {}

        void update(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime) override {
            if (currentPath_.empty()) {
                currentPath_ = findPath(harvester->getPosition(), targetPosition_, map);
                if (currentPath_.empty()) {
                    map.addSystemMessage(L"Unable to find path to target.");
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
                    // 목적지 도달
                    ai_->changeState(std::make_unique<IdleState>(ai_));
                }
            }
        }

        std::wstring getStateName() const override { return L"Moving"; }

    private:
        HarvesterAI* ai_;
        types::Position targetPosition_;
        std::vector<types::Position> currentPath_;
    };

    /**
     * @brief 수확 위치로 이동 중인 상태 클래스입니다.
     */
    class MovingToHarvestState : public HarvesterState {
    public:
        MovingToHarvestState(HarvesterAI* ai, const types::Position& spicePos)
            : ai_(ai), spicePosition_(spicePos) {}

        void update(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime) override {
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

        std::wstring getStateName() const override { return L"MovingToHarvest"; }

    private:
        HarvesterAI* ai_;
        types::Position spicePosition_;
        std::vector<types::Position> currentPath_;
    };

    /**
     * @brief 수확 중인 상태 클래스입니다.
     */
    class HarvestingState : public HarvesterState {
    public:
        explicit HarvestingState(HarvesterAI* ai)
            : ai_(ai), harvestStartTime_(std::chrono::milliseconds(0)) {}

        void update(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime) override {
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

        std::wstring getStateName() const override { return L"Harvesting"; }

    private:
        static constexpr auto HARVEST_TIME = std::chrono::seconds(4);
        static constexpr int MIN_HARVEST = 2;
        static constexpr int MAX_HARVEST = 4;

        HarvesterAI* ai_;
        std::chrono::milliseconds harvestStartTime_;
    };

    /**
     * @brief 본진으로 돌아가는 상태 클래스입니다.
     */
    class ReturningState : public HarvesterState {
    public:
        explicit ReturningState(HarvesterAI* ai) : ai_(ai) {}

        void update(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime) override {
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

        std::wstring getStateName() const override { return L"Returning"; }

    private:
        HarvesterAI* ai_;
        std::vector<types::Position> currentPath_;
    };

} // namespace dune::entity