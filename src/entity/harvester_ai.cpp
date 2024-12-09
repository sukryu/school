#include "entity/harvester_ai.hpp"
#include "entity/harvester_state.hpp"
#include "entity/unit.hpp"
#include "core/map.hpp"
#include "utils/harvester_command.hpp"

namespace dune::entity {

    HarvesterAI::HarvesterAI(const types::Position& basePosition)
            : basePosition_(basePosition)
            , currentState_(std::make_unique<IdleState>(this))
            , spiceAmount_(0) {}
    
    void HarvesterAI::update(Unit* harvester, core::Map& map, std::chrono::milliseconds currentTime) {
        if (currentState_) {
            currentState_->update(harvester, map, currentTime);
        }

        // 명령이 없고 유후 상태일 때 마지막 명령 반복 실행
        if (commandQueue_.hasCommand() &&
        currentState_->getStateName() == L"Idle") {
            executeLastCommand(harvester, map, currentTime);
        }
    }

    bool HarvesterAI::giveHarvestCommand(
        Unit* harvester, core::Map& map,
        const types::Position& spicePosition,
        std::chrono::milliseconds currentTime
    ) {
        const auto& terrain = map.getTerrainManager().getTerrain(spicePosition);
        if (terrain.getType() != types::TerrainType::Spice) {
            map.addSystemMessage(L"Invalid harvest location: No spice found.");
            return false;
        }

        if (isSpiceOccupied(map, spicePosition)) {
            map.addSystemMessage(L"This spice field is already being harvested.");
            return false;
        }

        auto command = HarvesterCommand::createHarvestCommand(spicePosition, currentTime);
        commandQueue_.addCommand(command);
        targetPosition_ = spicePosition;
        changeState(std::make_unique<MovingToHarvestState>(this, spicePosition));
        return true;
    }

    bool HarvesterAI::giveMoveCommand(
        Unit* harvester, 
        core::Map& map,
        const types::Position& movePosition,
        std::chrono::milliseconds currentTime
    ) {
        if (!isValidMovePosition(map, movePosition)) {
            map.addSystemMessage(L"Cannot move to this location.");
            return false;
        }

        auto command = HarvesterCommand::createMoveCommand(movePosition, currentTime);
        commandQueue_.addCommand(command);
        targetPosition_ = movePosition;
        changeState(std::make_unique<MovingState>(this, movePosition));
        return true;
    }

    void HarvesterAI::executeLastCommand(
        Unit* harvester,
        core::Map& map,
        std::chrono::milliseconds currentTime
    ) {
        const auto& command = commandQueue_.getCurrentCommand();
        switch (command.type) {
            case HarvesterCommand::Type::Harvest:
                giveHarvestCommand(harvester, map, command.targetPosition, currentTime);
                break;
            case HarvesterCommand::Type::Move:
                giveMoveCommand(harvester, map, command.targetPosition, currentTime);
                break;
            default:
                break;
        }
    }

    bool HarvesterAI::isSpiceOccupied(
        const core::Map& map,
        const types::Position& spicePos
    ) const {
        const auto& units = map.getUnitManager().getUnits();
        for (const auto& [pos, unit] : units) {
            if (unit->getType() == types::UnitType::Harvester &&
                unit->getPosition() == spicePos) {
                    return true;
            }
        }
        return false;
    }

    bool HarvesterAI::isValidMovePosition(
        const core::Map& map,
        const types::Position& pos
    ) const {
        if (!pos.is_valid()) return false;

        const auto& terrain = map.getTerrainManager().getTerrain(pos);
        return terrain.isWalkable() && !map.getEntityAt<Building>(pos);
    }
}