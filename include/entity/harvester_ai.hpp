#pragma once
#include <memory>
#include <string>
#include <chrono>
#include "harvester_state.hpp"
#include "utils/harvester_command.hpp"

namespace dune::entity {
    /**
     * @brief 하베스터의 AI를 관리하는 클래스입니다.
     */
    class HarvesterAI {
    public:
        /**
         * @brief HarvesterAI 생성자
         * @param basePosition 본진의 위치
         */
        HarvesterAI(const types::Position& basePosition);

        /**
         * @brief AI 상태를 업데이트합니다.
         */
        void update(Unit* harvester, core::Map& map, std::chrono::milliseconds currentTime);

        /**
         * @brief 수확 명령을 내립니다.
         */
        bool giveHarvestCommand(Unit* harvester, core::Map& map,
            const types::Position& spicePosition,
            std::chrono::milliseconds currentTime);

        /**
         * @brief 이동 명령을 내립니다.
         */
        bool giveMoveCommand(Unit* harvester, core::Map& map,
            const types::Position& movePosition,
            std::chrono::milliseconds currentTime);

        // 이전과 동일한 접근자 메서드들
        inline std::wstring getCurrentState() const {
            return currentState_ ? currentState_->getStateName() : L"Unknown";
        }

        void changeState(std::unique_ptr<HarvesterState> newState);

        inline const HarvesterCommand& getCurrentCommand() const {
            return commandQueue_.getCurrentCommand();
        }

        inline types::Position getBasePosition() const { return basePosition_; }
        inline types::Position getTargetPosition() const { return targetPosition_; }
        inline int getSpiceAmount() const { return spiceAmount_; }
        inline void setSpiceAmount(int amount) { spiceAmount_ = amount; }

    private:
        /**
         * @brief 마지막 명령을 다시 실행합니다.
         */
        void executeLastCommand(Unit* harvester, core::Map& map,
            std::chrono::milliseconds currentTime);

        /**
         * @brief 스파이스 매장지 점유 여부를 확인합니다.
         */
        bool isSpiceOccupied(const core::Map& map, const types::Position& spicePos) const;

        /**
         * @brief 이동 가능 위치인지 확인합니다.
         */
        bool isValidMovePosition(const core::Map& map, const types::Position& pos) const;

        types::Position basePosition_;      // 본진 위치
        types::Position targetPosition_;    // 목표 위치
        std::unique_ptr<HarvesterState> currentState_;  // 현재 상태
        HarvesterCommandQueue commandQueue_; // 명령 큐
        int spiceAmount_;                   // 현재 보유 스파이스량
    };
} // namespace dune::entity