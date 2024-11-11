#pragma once
#include "../utils/types.hpp"

namespace dune {
    namespace core {
        class Entity {
        public:
            virtual ~Entity() = default;

            // 기본 인터페이스
            virtual wchar_t get_representation() const = 0;
            virtual int get_color() const = 0;
            virtual void print_info() const = 0;
            virtual types::Position get_position() const = 0;
            virtual void move(types::Direction d) = 0;

            // 공통 유틸리티 메서드
            bool is_at(const types::Position& pos) const {
                return get_position() == pos;
            }

            virtual bool is_selectable() const {
                return true;
            }

        protected:
            Entity() = default;
            Entity(const Entity&) = default;
            Entity& operator=(const Entity&) = default;
        };
    } // namespace core
} // namespace dune