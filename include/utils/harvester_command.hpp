#pragma once
#include "types.hpp"
#include <chrono>

namespace dune::entity {
    /**
     * @brief 하베스터의 명령을 나타내는 구조체입니다.
     */
    struct HarvesterCommand {
        /**
         * @brief 하베스터가 수행할 수 있는 명령 타입입니다.
         */
        enum class Type {
            None,       // 명령 없음
            Harvest,    // 스파이스 수확
            Move,       // 이동
        };

        /**
         * @brief 기본 생성자
         */
        HarvesterCommand()
            : type(Type::None)
            , targetPosition({ 0, 0 })
            , issueTime(std::chrono::milliseconds(0)) {}

        /**
         * @brief 명령 생성자
         * @param commandType 명령 타입
         * @param position 목표 위치
         * @param time 명령 발급 시간
         */
        HarvesterCommand(Type commandType,
            const types::Position& position,
            std::chrono::milliseconds time)
            : type(commandType)
            , targetPosition(position)
            , issueTime(time) {}

        /**
         * @brief 명령이 유효한지 확인합니다.
         */
        bool isValid() const {
            return type != Type::None && targetPosition.is_valid();
        }

        /**
         * @brief 수확 명령을 생성합니다.
         */
        static HarvesterCommand createHarvestCommand(
            const types::Position& spicePosition,
            std::chrono::milliseconds currentTime) {
            return HarvesterCommand(Type::Harvest, spicePosition, currentTime);
        }

        /**
         * @brief 이동 명령을 생성합니다.
         */
        static HarvesterCommand createMoveCommand(
            const types::Position& movePosition,
            std::chrono::milliseconds currentTime) {
            return HarvesterCommand(Type::Move, movePosition, currentTime);
        }

        /**
         * @brief 명령 타입을 문자열로 반환합니다.
         */
        std::wstring getTypeString() const {
            switch (type) {
            case Type::Harvest: return L"Harvest";
            case Type::Move: return L"Move";
            default: return L"None";
            }
        }

        Type type;                              // 명령 타입
        types::Position targetPosition;         // 목표 위치
        std::chrono::milliseconds issueTime;    // 명령 발급 시간
    };

    /**
     * @brief 명령 큐를 관리하는 클래스입니다.
     */
    class HarvesterCommandQueue {
    public:
        /**
         * @brief 새로운 명령을 추가합니다.
         */
        void addCommand(const HarvesterCommand& command) {
            currentCommand_ = command;
        }

        /**
         * @brief 현재 명령을 가져옵니다.
         */
        const HarvesterCommand& getCurrentCommand() const {
            return currentCommand_;
        }

        /**
         * @brief 명령을 클리어합니다.
         */
        void clear() {
            currentCommand_ = HarvesterCommand();
        }

        /**
         * @brief 실행 중인 명령이 있는지 확인합니다.
         */
        bool hasCommand() const {
            return currentCommand_.isValid();
        }

    private:
        HarvesterCommand currentCommand_;  // 현재 실행 중인 명령
    };
}