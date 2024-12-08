#pragma once
#include <memory>
#include <string>
#include <chrono>
#include "sandworm_state.hpp"

// 전방 선언
namespace dune {
    namespace core { class Map; }
    namespace entity {
        class Unit;
        class SandwormState;
    }
}

namespace dune::entity {

    class SandwormAI {
    public:
        SandwormAI();
        ~SandwormAI();
        void update(Unit* sandworm, core::Map& map, std::chrono::milliseconds currentTime);
        std::wstring getCurrentState() const;
        void changeState(std::unique_ptr<SandwormState> newState);

    private:
        std::unique_ptr<SandwormState> currentState_;
    };

} // namespace dune::entity
