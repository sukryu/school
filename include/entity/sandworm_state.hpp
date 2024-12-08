#pragma once
#include "utils/types.hpp"
#include <chrono>
#include <vector>
#include <string>

// 전방 선언
namespace dune {
    namespace core { class Map; }
    namespace entity { class SandwormAI; class Unit; }
}

namespace dune::entity {

    class SandwormState {
    public:
        virtual ~SandwormState() = default;
        virtual void update(Unit* sandworm, core::Map& map, std::chrono::milliseconds currentTime) = 0;
        virtual std::wstring getStateName() const = 0;

    protected:
        bool isValidTarget(const Unit* target) const;
        types::Position findNearestPrey(const Unit* sandworm, const core::Map& map) const;
        types::Position findSuitableExcretionSpot(const types::Position& currentPos, const core::Map& map) const;
        std::vector<types::Position> findPath(
            const types::Position& start,
            const types::Position& goal,
            const dune::core::Map& map
        ) const;

        void changeState(SandwormAI* ai, std::unique_ptr<SandwormState> newState);
    };

    class HuntingState : public SandwormState {
    public:
        explicit HuntingState(SandwormAI* ai) : ai_(ai) {}
        void update(Unit* sandworm, core::Map& map, std::chrono::milliseconds currentTime) override;
        std::wstring getStateName() const override { return L"Hunting"; }

    private:
        SandwormAI* ai_;
        std::vector<types::Position> currentPath_;
        std::chrono::milliseconds lastPathUpdate_{ 0 };
        static constexpr auto PATH_UPDATE_INTERVAL = std::chrono::seconds(3);
        bool isValidMovePosition(const types::Position& pos, const core::Map& map) const;
    };

    class DigestingState : public SandwormState {
    public:
        explicit DigestingState(SandwormAI* ai) : ai_(ai), digestStartTime_(std::chrono::milliseconds(0)) {}
        void update(Unit* sandworm, core::Map& map, std::chrono::milliseconds currentTime) override;
        std::wstring getStateName() const override { return L"Digesting"; }

    private:
        SandwormAI* ai_;
        std::chrono::milliseconds digestStartTime_;
        static constexpr auto DIGESTION_TIME = std::chrono::seconds(5);
    };

    class BurrowingState : public SandwormState {
    public:
        explicit BurrowingState(SandwormAI* ai) : ai_(ai), burrowStartTime_(std::chrono::milliseconds(0)) {}
        void update(Unit* sandworm, core::Map& map, std::chrono::milliseconds currentTime) override;
        std::wstring getStateName() const override { return L"Burrowing"; }

    private:
        SandwormAI* ai_;
        std::chrono::milliseconds burrowStartTime_;
        static constexpr auto BURROW_TIME = std::chrono::seconds(2);
        types::Position burrowTarget_;
    };

} // namespace dune::entity
