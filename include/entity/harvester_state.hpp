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
        bool isValidPosition(const types::Position& pos, const core::Map& map) const;
    };

    /**
     * @brief 대기 상태 클래스입니다.
     */
    class IdleState : public HarvesterState {
    public:
        explicit IdleState(HarvesterAI* ai);

        void update(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime) override;

        std::wstring getStateName() const override { return L"Idle"; }

    private:
        HarvesterAI* ai_;
    };

    /**
     * @brief 이동 중인 상태 클래스입니다.
     */
    class MovingState : public HarvesterState {
    public:
        MovingState(HarvesterAI* ai, const types::Position& target);

        void update(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime) override;

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
        MovingToHarvestState(HarvesterAI* ai, const types::Position& spicePos);

        void update(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime) override;

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
        explicit HarvestingState(HarvesterAI* ai);

        void update(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime) override;

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
        explicit ReturningState(HarvesterAI* ai);

        void update(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime) override;

        std::wstring getStateName() const override { return L"Returning"; }

    private:
        HarvesterAI* ai_;
        std::vector<types::Position> currentPath_;
    };

} // namespace dune::entity