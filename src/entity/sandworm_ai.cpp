#include "entity/sandworm_ai.hpp"
#include "entity/sandworm_state.hpp"
#include "entity/unit.hpp"
#include "core/map.hpp"

namespace dune::entity {
    SandwormAI::SandwormAI()
        : currentState_(std::make_unique<HuntingState>(this)) {}

    SandwormAI::~SandwormAI() = default;

    void SandwormAI::update(Unit* sandworm, core::Map& map, std::chrono::milliseconds currentTime) {
        if (currentState_) {
            currentState_->update(sandworm, map, currentTime);
        }
    }

    std::wstring SandwormAI::getCurrentState() const {
        return currentState_ ? currentState_->getStateName() : L"Unknown";
    }

    void SandwormAI::changeState(std::unique_ptr<SandwormState> newState) {
        currentState_ = std::move(newState);
    }
}
